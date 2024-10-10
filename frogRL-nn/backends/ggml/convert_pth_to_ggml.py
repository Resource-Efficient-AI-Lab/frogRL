import struct
import json
import gguf
import sys
import numpy as np
import argparse

import torch
import torch.nn as nn


def parse_args():
    parser = argparse.ArgumentParser(description="Convert .pth of a DRL model to ggml.")
    parser.add_argument("--model-name", type=str, help="model input path")
    group = parser.add_mutually_exclusive_group()
    group.add_argument(
        "-c", "--continous", action="store_true", help="continuos action space"
    )
    group.add_argument(
        "-d", "--discrete", action="store_true", help="discrete action space"
    )
    parser.add_argument("--algo", type=str.lower, help="algorithm used")
    parser.add_argument("--env", type=str.lower, help="environment name")
    return parser


def convert_pth_to_gguf(state_dict, args) -> dict:
    match args.algo:
        case "ppo":
            return convert_ppo(state_dict, args)
        case "ddpg":
            return convert_ddpg(state_dict, args)
        case "dqn":
            return convert_dqn(state_dict, args)
        case "sac":
            return convert_sac(state_dict, args)
        case "td3":
            return convert_td3(state_dict, args)
        case _:
            raise ValueError("algorithm not supported")


def convert_ppo(state_dict: dict, args):
    if args.discrete:
        return {
            k: v
            for k, v in state_dict.items()
            if k.startswith("network.") or k.startswith("actor.")
        }
    else:
        return {
            k: v
            for k, v in state_dict.items()
            if k.startswith("actor_mean.") or k == "actor_logstd"
        }


def convert_ddpg(state_dicts, args):
    if args.discrete:
        raise ValueError(
            "ddpg is used for continous action space. For discrete action space, use DQN or others."
        )
    actor_state_dict, _ = state_dicts
    return {
        k: v
        for k, v in actor_state_dict.items()
        if k.startswith("fc") or k.startswith("action_")
    }


def convert_dqn(state_dict, args):
    return {}


def convert_td3(state_dicts, args):
    if args.discrete:
        raise ValueError(
            "td3 is used for continous action space. For discrete action space, use DQN or others."
        )
    actor_dict, _, _ = state_dicts
    return {
        k: v
        for k, v in actor_dict.items()
        if k.startswith("fc") or k.startswith("action_")
    }


def convert_sac(state_dict, args):
    actor_dict, _, _ = state_dict
    if args.discrete:
        return {
            k: v
            for k, v in actor_dict.items()
            if k.startswith("conv") or k.startswith("fc")
        }
    else:
        return {
            k: v
            for k, v in actor_dict.items()
            if k.startswith("fc") or k.startswith("action_")
        }


def is_fc_layer(name, param):
    return "weight" in name and param.dim() == 2


def get_additional_tensors(state_dict, args) -> dict:
    match args.algo:
        case "ppo":
            return {k: v for k, v in state_dict.items() if k.startswith("actor_logstd")}
        case _:
            raise ValueError("algorithm not supported")


if __name__ == "__main__":
    parser = parse_args()
    args = parser.parse_args()
    discrete = "discrete" if args.discrete else "continous"
    output_file = f"./ggml-{args.algo}-{discrete}-{args.env}.gguf"

    state_dict = torch.load(args.model_name, map_location=torch.device("cpu"))
    actor_dict = convert_pth_to_gguf(state_dict, args)

    writer = gguf.GGUFWriter(output_file, f"{args.algo}-{args.env}")

    fc_layers = [
        (name, param) for name, param in actor_dict.items() if is_fc_layer(name, param)
    ]

    for i, (weight_name, weight) in enumerate(fc_layers):
        in_features, out_features = weight.shape
        layer_id = i + 1

        writer.add_tensor(f"mlp.layer{layer_id}.weight", weight.numpy())
        bias_name = weight_name.replace("weight", "bias")
        if bias_name in actor_dict:
            writer.add_tensor(
                f"mlp.layer{layer_id}.bias", actor_dict[bias_name].numpy()
            )

    additional_tensors = ["actor_logstd"]

    for name in additional_tensors:
        if name in actor_dict:
            writer.add_tensor(name, actor_dict[name].numpy())

    writer.write_header_to_file()
    writer.write_kv_data_to_file()
    writer.write_tensors_to_file()
    writer.close()
