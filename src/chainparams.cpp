// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX Developers, The FIX Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

void MineGenesisBlock(CBlock &genesis)
{
	uint256 besthash = uint256();
	int n=0;
	uint256 hashTarget = uint256().SetCompact(genesis.nBits);
while (uint256(genesis.GetHash()) > hashTarget) {
	uint256 c = uint256(genesis.GetHash());

	if(c < besthash || n==0)
	{
		besthash = c;
		n=1;
		printf("%s %s %s\n",genesis.GetHash().GetHex().c_str(),hashTarget.GetHex().c_str(),besthash.GetHex().c_str());
	}
		++genesis.nNonce;
		if (genesis.nNonce == 0) { ++genesis.nTime; }
	}
//printf("HASH IS: %s\n", UintToArith256(genesis.GetHash()).ToString().c_str());
	printf("Converting genesis hash to string: %s\n",genesis.ToString().c_str()); 
}

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions

static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0x00000794bd15081527d28d5073cc8c3e5773bc0301a9ff8f6a25e2c40e6ff78b"))
    (50, uint256("0x000005b4af867ef5cd628a89c39d072e75cbc31fe12ec7cda5f0fe4480e64c92"))
    (100, uint256("0x0000040f52213032943a47c8ae395e9da1b668ea8c83365e9433b04262b01265"))
    (114, uint256("0x000005f70a40be7edd86131f42335b6a6dabd5f61c1904dc708d9264bcc61e3d"));

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1527091355, // * UNIX timestamp of last checkpoint block
    1,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    100        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1454124731,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1527091355,
    0,
    100};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xb1;
        pchMessageStart[1] = 0x2a;
        pchMessageStart[2] = 0xa3;
        pchMessageStart[3] = 0xe2;
        vAlertPubKey = ParseHex("042aaee5ca5468608d1df8a7a2d01ff1d4af66a8d35dd0e8900322b4fc427aceec5943be2876e61876079fd76cbdfb2784262aa4ec84a5cfeab9e6c605c9dd67a4");
        nDefaultPort = 31001;		//12457 for safety swap
        bnProofOfWorkLimit = ~uint256(0) >> 20; // FIX starting difficulty is 1 / 2^12
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // FIX: 60 seconds
        nTargetSpacing = 1 * 60;  // FIX: 60 seconds
        nLastPOWBlock = 515;
        nMaturity = 30;
        nMasternodeCountDrift = 20;
        nModifierUpdateBlock = 510;
        nMaxMoneyOut = 21000000 * COIN;

        const char* pszTimestamp = "Stephen Hawking, science's brightest star, His knowledge never dies";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 50 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04db7dab5e593c87aff87c77991a104830b612f2b791dd9d2292503959907b358f648bd525ba73e5fa22dc2234429a58da7e6014588dc1f5253749a7218b2a3ccf") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1527091355;	///23/05/2018
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 676811;

	bool startNewChain = false;
	if(startNewChain == true) { MineGenesisBlock(genesis); }

        hashGenesisBlock = genesis.GetHash();

	if(!startNewChain){

        	assert(hashGenesisBlock == uint256("0x00000794bd15081527d28d5073cc8c3e5773bc0301a9ff8f6a25e2c40e6ff78b"));
        	assert(genesis.hashMerkleRoot == uint256("0xf54ee24c17c4edeb780e300411ca689c58b09e41b6b4c357ef1e65faa34a4d39"));
	}

        vSeeds.push_back(CDNSSeedData("prufus one","118.69.37.45"));
        vSeeds.push_back(CDNSSeedData("prufus two","45.76.228.106"));
	vSeeds.push_back(CDNSSeedData("prufus two","45.76.29.178"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 55);	//Start at P
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 15);	//Start at 7
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 174);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x24)(0x33)(0x24).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x25)(0x32).convert_to_container<std::vector<unsigned char> >();
        //  BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x90)(0x32)(0x00)(0x83).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "04db7dab5e593c87aff87c77991a104830b612f2b791dd9d2292503959907b358f648bd525ba73e5fa22dc2234429a58da7e6014588dc1f5253749a7218b2a3ccf";
        strObfuscationPoolDummyAddress = "ETVDjeQDvFXz2gmip9NDN3Qcm295h4Kh7o";
        nStartMasternodePayments = 1527141169;	//Start payment MN from 24/05/2018
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x23;
        pchMessageStart[1] = 0x3e;
        pchMessageStart[2] = 0xa8;
        pchMessageStart[3] = 0xc5;
        vAlertPubKey = ParseHex("04db7dab5e593c87aff87c77991a104830b612f2b791dd9d2292503959907b358f648bd525ba73e5fa22dc2234429a58da7e6014588dc1f5253749a7218b2a3ccf");
        nDefaultPort = 14249;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // FIX: 1 minute
        nTargetSpacing = 1 * 60;  // FIX: 1 minute
        nLastPOWBlock = 200;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 43199500 * COIN;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1454124731;
        genesis.nNonce = 2402015;

        hashGenesisBlock = genesis.GetHash();

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet prufus addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet prufus script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet prufus BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet prufus BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet prufus BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();


        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "0421621928F4116C5B1562CE13621D0571F07B3134F14858B1D75B8A997A1C62FCAA29C59D398E86288C835A0B8B58587868C23B06174BA1A1DDC9A7A5239F072F";
        strObfuscationPoolDummyAddress = "ETVDjeQDvFXz2gmip9NDN3Qcm295h4Kh7o";
        nStartMasternodePayments = 1521364102; //Fri, 09 Jan 2015 21:05:58 GMT
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0x7e;
        pchMessageStart[3] = 0xac;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // Prufus: 1 day
        nTargetSpacing = 1 * 60;        // Prufus: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1454124731;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 12345;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 11476;
        //assert(hashGenesisBlock == uint256("0x4f023a2120d9127b21bbad01724fdb79b519f593f2a85b60d3d79160ec5f29df"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 11478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
