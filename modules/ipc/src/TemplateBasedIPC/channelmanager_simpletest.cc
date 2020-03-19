#include "gtest/gtest.h"

#include "modules/ipc/include/TemplateBasedIPC/channelmanager_simple.h"



namespace atd
{
namespace ipc
{



TEST(channelmanager_simple_Test, decodeEncodeTest)
{
    while (1) {
        std::shared_ptr<atd::map::EngineTargetLanes> targetLane(std::make_shared<atd::map::EngineTargetLanes>());
        RxMsgPtr<atd::map::EngineTargetLanes>(CHANNEL_NAME_PROTO_EngineTargetLanesBuffer,targetLane,true);
        auto seqNum=targetLane->header().sequence_num();
        uint8_t* dt_array_25000=new uint8_t[25000];
        dumpToArray<atd::map::EngineTargetLanes>(*targetLane,dt_array_25000);
//        DecodeAndEncodeFromArray::dumpToArray<atd::map::EngineTargetLanes>(targetLane,dt_array_25000);
        atd::map::EngineTargetLanes targetLaneDecode;
        decodeFromArray<atd::map::EngineTargetLanes>(targetLaneDecode,dt_array_25000);
//        DecodeAndEncodeFromArray::decodeFromArray<atd::map::EngineTargetLanes>(targetLaneDecode,dt_array_25000);
        EXPECT_TRUE(seqNum==(targetLaneDecode.header().sequence_num()));
        delete [] dt_array_25000;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);

    // Runs all tests using Google Test.
    return RUN_ALL_TESTS();
 }

}
}
