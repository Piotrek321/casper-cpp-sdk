#include <iostream>

#include "../src/include/CasperClient.h"  // To use Casper::Client features
#include "../src/include/Types/GlobalStateKey.h"

/// Construct a Casper::Client object
Casper::Client casper_client(CASPER_TEST_ADDRESS);

/// “info_get_peers” RPC.
void infoGetPeers() {
  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << "info_get_peers" << std::endl;
  /// Call GetNodePeers function from the client object.
  Casper::InfoGetPeersResult peers_result = casper_client.GetNodePeers();

  /// Print the nodes in the network.
  for (int i = 0; i < peers_result.peers.size(); i++) {
    /// Print the node ID.
    std::cout << i + 1 << ". Node ID: " << peers_result.peers[i].node_id;

    /// Print the node IP address.
    std::cout << " Address: " << peers_result.peers[i].address << "\n";
  }

  std::cout << std::endl;
}

/// “chain_get_state_root_hash” RPC.
void chainGetStateRootHash() {
  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << "chain_get_state_root_hash" << std::endl;

  /// Call GetStateRootHash function with the height of the block.
  Casper::GetStateRootHashResult height_result =
      casper_client.GetStateRootHash(10);
  std::cout << "state_root_hash for block height: "
            << height_result.state_root_hash << "\n";

  /// Call GetStateRootHash function with the block hash of the block.
  Casper::GetStateRootHashResult hash_result = casper_client.GetStateRootHash(
      "acc4646f35cc1d59b24381547a4d2dc1c992a202b6165f3bf68d3f23c2b93330");
  std::cout << "state_root_hash for given block hash: "
            << hash_result.state_root_hash << "\n";

  /// Call GetStateRootHash function with empty string.
  Casper::GetStateRootHashResult recent_hash_result =
      casper_client.GetStateRootHash();
  std::cout << "most recent state_root_hash: "
            << recent_hash_result.state_root_hash << "\n";

  std::cout << std::endl;
}

void infoGetDeploy() {
  // TODO: fill in this function
  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << "info_get_deploy" << std::endl;

  std::cout << std::endl;
}

void infoGetStatus() {
  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << "info_get_status" << std::endl;

  /// Call GetStatusInfo function.
  Casper::GetStatusResult status_result = casper_client.GetStatusInfo();
  std::cout << "\napi_version: " << status_result.api_version;
  std::cout << "\nchainspec_name: " << status_result.chainspec_name;
  std::cout << "\nstarting_state_root_hash: "
            << status_result.starting_state_root_hash;
  for (std::size_t i = 0; i < status_result.peers.size(); i++) {
    std::cout << std::to_string(i + 1)
              << "\nPeer: Address: " << status_result.peers[i].address
              << " - Node id:" << status_result.peers[i].node_id;
  }

  if (status_result.last_added_block_info.has_value()) {
    Casper::BlockInfo last_added_block_info =
        status_result.last_added_block_info.value();

    std::cout << "\nBlock info: creator: " << last_added_block_info.creator
              << "\nera_id: " << last_added_block_info.era_id
              << "\nhash: " << last_added_block_info.hash
              << "\nheight: " << last_added_block_info.height
              << "\nstate_root_hash: " << last_added_block_info.state_root_hash
              << "\ntimestamp: " << last_added_block_info.timestamp;
  }

  if (status_result.our_public_signing_key.has_value()) {
    std::cout << "\nour_public_signing_key: "
              << status_result.our_public_signing_key.value();
  }

  if (status_result.round_length.has_value()) {
    std::cout << "\nround_length: " << status_result.round_length.value();
  }

  if (status_result.next_upgrade.has_value()) {
    std::cout << "\next_upgrade: " << status_result.next_upgrade.value();
  }

  std::cout << "\nbuild_version: " << status_result.build_version;
  std::cout << "\nuptime: " << status_result.uptime;

  std::cout << std::endl;
}

void chainGetBlockTransfers() {
  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << "chain_get_block_transfers" << std::endl;

  /// Call GetBlockTransfers function.
  Casper::GetBlockTransfersResult transfersResult =
      casper_client.GetBlockTransfers(
          {"acc4646f35cc1d59b24381547a4d2dc1c992a202b6165f3bf68d3f23c2b93330",
           532041});
  std::cout << "\napi_version: " << transfersResult.api_version
            << "\nblock_hash: " << transfersResult.block_hash;
  for (std::size_t i = 0; i < transfersResult.transfers.size(); i++) {
    std::cout << std::to_string(i + 1) << "\nTransfer: deploy_hash: "
              << transfersResult.transfers[i].deploy_hash
              << "\nfrom:" << transfersResult.transfers[i].from
              << "\nto: " << transfersResult.transfers[i].to
              << "\nsource: " << transfersResult.transfers[i].source.ToString()
              << "\ntarget: " << transfersResult.transfers[i].target.ToString()
              << "\namount: " << transfersResult.transfers[i].amount
              << "\ngas: " << transfersResult.transfers[i].gas
              << "\nid: " << transfersResult.transfers[i].id;
  }

  std::cout << std::endl;
}

void chainGetBlock() {
  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << "chain_get_block" << std::endl;

  /// Call GetBlockResult rpc function.
  Casper::GetBlockResult blockResult = casper_client.GetBlock(
      "acc4646f35cc1d59b24381547a4d2dc1c992a202b6165f3bf68d3f23c2b93330");
  // or casper_client.GetBlock(532041);

  std::cout << "\napi_version: " << blockResult.api_version;

  if (!blockResult.block.body.proposer.empty()) {
    std::cout << "\nproposer: " << blockResult.block.body.proposer
              << "\nhash: " << blockResult.block.hash << "\naccumulated_seed: "
              << blockResult.block.header.accumulated_seed
              << "\nbody_hash: " << blockResult.block.header.body_hash
              << "\nera_end: " << blockResult.block.header.era_end
              << "\nera_id: " << blockResult.block.header.era_id
              << "\nheight: " << blockResult.block.header.height
              << "\nparent_hash: " << blockResult.block.header.parent_hash
              << "\nprotocol_version: "
              << blockResult.block.header.protocol_version
              << "\nrandom_bit: " << blockResult.block.header.random_bit
              << "\nstate_root_hash: "
              << blockResult.block.header.state_root_hash
              << "\ntimestamp: " << blockResult.block.header.timestamp;

    for (std::size_t i = 0; i < blockResult.block.body.deploy_hashes.size();
         i++) {
      std::cout << "\nDeploy hash: " << blockResult.block.body.deploy_hashes[i];
    }

    for (std::size_t i = 0; i < blockResult.block.body.transfer_hashes.size();
         i++) {
      std::cout << "\nTransfer hash: "
                << blockResult.block.body.transfer_hashes[i];
    }

    for (std::size_t i = 0; i < blockResult.block.proofs.size(); i++) {
      std::cout << "\nProof: PK: " << blockResult.block.proofs[i].public_key
                << "\nSignature: " << blockResult.block.proofs[i].signature
                << "\n";
    }
  }

  std::cout << std::endl;
}

void chainGetEraInfoBySwitchBlock() {
  std::cout << "-----------------------------------------------\n";
  std::cout << "chain_get_era_info_by_switch_block" << std::endl;

  /// Call chain_get_era_info_by_switch_block rpc function.
  Casper::GetEraInfoResult eraInfoResult =
      casper_client.GetEraInfoBySwitchBlock(
          "acc4646f35cc1d59b24381547a4d2dc1c992a202b6165f3bf68d3f23c2b93330");
  std::cout << "\napi_version: " << eraInfoResult.api_version;

  if (eraInfoResult.era_summary.has_value()) {
    Casper::EraSummary eraSummary = eraInfoResult.era_summary.value();

    std::cout << "\nblock_hash: " << eraSummary.block_hash
              << "\nera_id: " << eraSummary.era_id
              << "\nmerkle_proof: " << eraSummary.merkle_proof
              << "\nstate_root_hash: " << eraSummary.state_root_hash
              << "\nstored_value: ";
    // TODO: add a toString function to the stored value class
    // << eraInfoResult.era_summary.stored_value;  // TODO: std::visit ToString
  }

  std::cout << std::endl;
}

void stateGetItem() {
  // TODO: implement
  std::cout << "-----------------------------------------------\n";
  std::cout << "state_get_item" << std::endl;

  std::cout << std::endl;
}

void stateGetDictionaryItem() {
  // TODO: implement
  std::cout << "-----------------------------------------------\n";
  std::cout << "state_get_dictionary_item" << std::endl;

  std::cout << std::endl;
}

void stateGetBalance() {
  std::cout << "-----------------------------------------------\n";
  std::cout << "state_get_balance" << std::endl;

  /// Call state_get_balance rpc function.
  Casper::GetBalanceResult balanceResult = casper_client.GetAccountBalance(
      "uref-ca7f8a20800e189b3509952c84db91efa04df7bb73c43ae34a5598d88ac7a783-"
      "007",
      "f5abb3964382e0dde4bc3ec38414f43f325f5dcc6493d5a7c4037972793fb302");
  std::cout << "\nbalanceResult: " << balanceResult.balance_value;

  std::cout << std::endl;
}

void stateGetAuctionInfo() {
  // TODO: implement
  std::cout << "-----------------------------------------------\n";
  std::cout << "state_get_auction_info" << std::endl;

  /// Call GetStateRootHash function with the height of the block.
  Casper::GetAuctionInfoResult auction_result = casper_client.GetAuctionInfo(
      "9f246c64116b5c686c8e6a6829fce36c86bb32437866b617d91ed7de9f6a8a16");
  std::cout << "\nstate_root_hash: "
            << auction_result.auction_state.state_root_hash
            << "\nblock_height: " << auction_result.auction_state.block_height

            << "\nera_validators:";

  for (auto& validator : auction_result.auction_state.era_validators) {
    std::cout << "\n" << validator.era_id;

    // TODO: bid is missing
    // TODO: public key in validator_weights is missing

    std::cout << "\nValidator Weights: ";
    for (auto& weight : validator.validator_weights) {
      std::cout << "\n" << weight.public_key << " " << weight.weight.toString();
    }
  }

  for (auto& bid_res : auction_result.auction_state.bids) {
    if (bid_res.public_key.empty()) {
      std::cout << "\nbonding_purse: " << bid_res.bid.bonding_purse.ToString()
                << "\ndelegation_rate: "
                << unsigned(bid_res.bid.delegation_rate)
                << "\ninactive: " << std::boolalpha << bid_res.bid.inactive
                << "\nstaked_amount: " << bid_res.bid.staked_amount.toString()
                << "\nvalidator_public_key: " << bid_res.public_key;
    } else {
      std::cout << "\npublic_key: " << bid_res.public_key;
    }
  }

  std::cout << std::endl;
}

int main() {
  infoGetPeers();

  chainGetStateRootHash();

  // Milestone 2
  infoGetDeploy();

  infoGetStatus();

  chainGetBlockTransfers();

  chainGetBlock();

  chainGetEraInfoBySwitchBlock();

  stateGetItem();

  stateGetDictionaryItem();

  stateGetBalance();

  stateGetAuctionInfo();

  // Milestone 3
  // PutDeploy(); TODO: implement
}