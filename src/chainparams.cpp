// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2018 Supermoon developers 
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
// Build PROD v1.1 Wallet 1.1.0.2 (Sep29-18)  
#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

void MineGenesis(CBlock genesis){
    uint256 hashTarget = CBigNum().SetCompact(Params().ProofOfWorkLimit().GetCompact()).getuint256();
    printf("Target: %s\n", hashTarget.GetHex().c_str());
    uint256 newhash = genesis.GetHash();
    uint256 besthash;
    memset(&besthash,0xFF,32);
    while (newhash > hashTarget) {
    	++genesis.nNonce;
        if (genesis.nNonce == 0){
            printf("NONCE WRAPPED, incrementing time");
            ++genesis.nTime;
        }
	newhash = genesis.GetHash();
	if(newhash < besthash){
	    besthash=newhash;
	    printf("New best: %s\n", newhash.GetHex().c_str());
	}
    }
    printf("Found Genesis, Nonce: %1d, Hash: %s\n", genesis.nNonce, genesis.GetHash().GetHex().c_str());
    printf("Gensis Hash Merkle: %s\n", genesis.hashMerkleRoot.ToString().c_str());
}

//
// Main network
//

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // Message start string
        pchMessageStart[0] = 0xcd;
        pchMessageStart[1] = 0xb1;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xeb;
        vAlertPubKey = ParseHex("0471d2a6eef17ee2b4ab24480cc11ee46f7867ad61544e24a6b9724983868049b7078d8341da9440c9700c49afd027da3b60b6d02b26438d323e5f05942e89e527");
     
	nDefaultPort = 16998;
        nRPCPort = 16999;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);

        // Build the genesis. 
        const char* pszTimestamp = "Supermoon-V1-1-Production1Sep2018";
        std::vector<CTxIn> vin;
        vin.resize(1);
        vin[0].scriptSig = CScript() << 0 << CBigNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        std::vector<CTxOut> vout;
        vout.resize(1);
        vout[0].SetEmpty();
        CTransaction txNew(1, 1535953653, vin, vout, 0);
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1535953653;
        genesis.nBits    = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce   = 1925456;

	//MineGenesis(genesis);    ////Mine merkleroot + Genesis block hash+nonce
        
	hashGenesisBlock = genesis.GetHash();
          assert(hashGenesisBlock == uint256("0x0000057c2413b80a910c03e9f7e40339ba6577fbbf46e1ac4e90766d147aa0cf"));
          assert(genesis.hashMerkleRoot == uint256("0x030c5a65bdea9543b5d28cd49af8392d44aca44a2e8a5797ee61c235d5cb0cd8"));
      
	
       	vSeeds.push_back(CDNSSeedData("45.76.124.145", "45.76.124.145")); 	// SUPM-Node1   (PROD)
	vSeeds.push_back(CDNSSeedData("207.246.103.227", "207.246.103.227")); 	// SUPM-Node2	(PROD)	
	vSeeds.push_back(CDNSSeedData("199.247.16.65", "199.247.16.65")); 	// SUPM-Node3	(PROD)	
	//vSeeds.push_back(CDNSSeedData("45.76.118.86", "45.76.118.86")); 	// SUPM-DEV1   (DEV)    
		
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 63);	//Addys Start with S
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 83);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1, (63+128));
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xC2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0xDD).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        nLastPOWBlock = 3500;		//validation point for last possible Mining
        nPOSStartBlock = 3501;		//validation point for staking to start
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet
//

class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string
        pchMessageStart[0] = 0xce;
        pchMessageStart[1] = 0xd2;
        pchMessageStart[2] = 0xb6;
        pchMessageStart[3] = 0x5c;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);
       	vAlertPubKey = ParseHex("04f932a7de828635a444e7a055348e2d6f5d679b9c2af7aeca9b05132ed5f32c1f84bf1f040458925fb16c846d7c176d27c0073723ace7aa759fa4fe73b84eafb8");        

        nDefaultPort = 19998;
        nRPCPort = 19999;
        strDataDir = "testnet";
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 4119810;
        genesis.nTime    = 1535953950;
  	//Mine test + Genesis block hash+nonce (merkle done already above)         
	//MineGenesis(genesis);   
	
	hashGenesisBlock = genesis.GetHash();
                assert(hashGenesisBlock == uint256("0x000009606fc76f13afb43a7ae23f778825f94aa3a410e1b2cd7b948ea5a8d718"));

        vFixedSeeds.clear(); 
        vSeeds.clear();

        vSeeds.push_back(CDNSSeedData("45.76.124.145", "45.76.124.145")); 	// SUPM-Node1   (PROD)
	vSeeds.push_back(CDNSSeedData("207.246.103.227", "207.246.103.227")); 	// SUPM-Node2	(PROD)	
	vSeeds.push_back(CDNSSeedData("199.247.16.65", "199.247.16.65")); 	// SUPM-Node3	(PROD)	 
	//vSeeds.push_back(CDNSSeedData("45.76.118.86", "45.76.118.86")); 	// SUPM-DEV1   (DEV) 

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 65); // Supermoon testnet Addys Start with T
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1, 65 + 128);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        nLastPOWBlock = 0x7fffffff;
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        pchMessageStart[0] = 0xef;
        pchMessageStart[1] = 0xd3;
        pchMessageStart[2] = 0xb7;
        pchMessageStart[3] = 0x7d;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        genesis.nTime = 1535987985;
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 23552;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 21999;
        strDataDir = "regtest";

        //MineGenesis(genesis);  //mine last nTime setting Genesis block
        assert(hashGenesisBlock == uint256("0x00000590367dfecb2a34242b4173e91bb07e8c012f83ac1e2bb1fd3f0196ead4"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
