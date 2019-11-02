/**
 *  @copyright Copyright 2019 The J-PET Framework Authors. All rights reserved.
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may find a copy of the License in the LICENCE file.
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  @file JPetChannelTest.cpp
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE JPetChannelTest

#include <boost/test/unit_test.hpp>
#include "JPetChannelFactory.h"
#include "JPetChannel.h"

// TODO finish

float epsilon = 0.0001;

BOOST_AUTO_TEST_SUITE(ClassSuite)

BOOST_AUTO_TEST_CASE(default_constructor)
{
  JPetChannel channel;
  BOOST_REQUIRE_EQUAL(channel.getID(), -1);
  BOOST_REQUIRE_EQUAL(channel.getThresholdNumber(), 0);
  BOOST_REQUIRE_CLOSE(channel.getThresholdValue(), 0.0, epsilon);
}

BOOST_AUTO_TEST_CASE(constructor)
{
  JPetChannel channel(1, 3, 100.0);
  BOOST_REQUIRE_EQUAL(channel.getID(), 1);
  BOOST_REQUIRE_EQUAL(channel.getThresholdNumber(), 3);
  BOOST_REQUIRE_CLOSE(channel.getThresholdValue(), 100.0, epsilon);
  JPetPM pm(57, JPetPM::SideA, "sipm", 3);
  channel.setPM(pm);
  BOOST_REQUIRE_EQUAL(channel.getPM().getID(), 57);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(FactorySuite)

class TestParamGetter: public JPetParamGetter
{

ParamObjectsDescriptions getAllBasicData(
    ParamObjectType type, const int runID
) {
  ParamObjectsDescriptions result;
  switch (type) {
    case ParamObjectType::kChannel:
      switch (runID) {
        // No Channels
        case 0:
          break;
        // Single object
        case 1:
          result = {
            {1, {	{"id", "1"}, {"thr_num", "1"}, {"thr_val", "80.0"}, {"pm_id", "1"} } }
          };
          break;
        // Two objects
        case 2:
          result = {
            {1, {	{"id", "1"}, {"thr_num", "1"}, {"thr_val", "80.0"}, {"pm_id", "1"} } },
            {5, {	{"id", "5"}, {"thr_num", "2"}, {"thr_val", "110.0"}, {"pm_id", "1"} } }
          };
          break;
        // Missing field
        case 3:
          result = {
            {1, {	{"id", "1"}, {"thr_num", "1"}, {"pm_id", "1"} } }
          };
          break;
        // Wrong field
        case 4:
          result = {
            {1, {	{"id", "1"}, {"thr_num", "last"}, {"thr_val", "80.0"}, {"pm_id", "1"} } }
          };
          break;
        // Wrong relation
        case 5:
          result = {
            {1, {	{"id", "1"}, {"thr_num", "33"}, {"thr_val", "80.0"}, {"pm_id", "22"} } }
          };
          break;
      }
      break;
    case ParamObjectType::kPM:
      result = {
        {1, { {"id", "1"}, {"side", "B"}, {"description", "nice"}, {"pos_in_matrix", "1"}, {"scin_id", "1"} } }
      };
      break;
    case ParamObjectType::kScin:
      result = {
        {1, { {"id", "1"},
          {"length", "50.0"}, {"width", "15.0"}, {"height", "7.0"},
          {"center_x", "5.0"}, {"center_y", "-5.0"}, {"center_z", "15.0"},
          {"slot_id", "1"}
        } }
      };
      break;
    case ParamObjectType::kSlot:
      result = { { 1, { {"id", "1"}, {"theta", "5.5"}, {"type", "module"}, {"layer_id", "1"} } } };
      break;
    case ParamObjectType::kLayer:
      result = { { 1, { {"id", "1"}, {"name", "sorbet"}, {"radius", "10.5"}, {"setup_id", "1"} } } };
      break;
    case ParamObjectType::kSetup:
      result = { { 1, { {"id", "1" }, {"description", "jpet"} } } };
      break;
    default:
      break;
  }
  return result;
}

ParamRelationalData getAllRelationalData(
    ParamObjectType type, ParamObjectType, const int runID
) {
  ParamRelationalData result;
  switch (type) {
    case ParamObjectType::kChannel:
      switch (runID) {
        // No relations
        case 0:
          break;
        // Single object
        case 1:
          result = { {1, 1} };
          break;
        // Two objects
        case 2:
          result = { {1, 1}, {5, 1} };
          break;
        // Missing field
        case 3:
          result = { {1, 1} };
          break;
        // Wrong field
        case 4:
          result = { {1, 1} };
          break;
        // Wrong relation
        case 5:
          result = { {1, 43} };
          break;
      }
      break;
    default:
      result = { {1, 1} };
      break;
  }
  return result;
}
};

TestParamGetter paramGetter;

BOOST_AUTO_TEST_CASE(no_Channels)
{
  JPetSetupFactory setupFactory(paramGetter, 0);
  JPetLayerFactory layerFactory(paramGetter, 0, setupFactory);
  JPetSlotFactory slotFactory(paramGetter, 0, layerFactory);
  JPetScinFactory scinFactory(paramGetter, 0, slotFactory);
  JPetPMFactory pmFactory(paramGetter, 0, scinFactory);
  JPetChannelFactory channelFactory(paramGetter, 0, pmFactory);
  auto& channels = channelFactory.getChannels();
  BOOST_REQUIRE_EQUAL(channels.size(), 0);
}

BOOST_AUTO_TEST_CASE(single_object)
{
  JPetSetupFactory setupFactory(paramGetter, 1);
  JPetLayerFactory layerFactory(paramGetter, 1, setupFactory);
  JPetSlotFactory slotFactory(paramGetter, 1, layerFactory);
  JPetScinFactory scinFactory(paramGetter, 1, slotFactory);
  JPetPMFactory pmFactory(paramGetter, 1, scinFactory);
  JPetChannelFactory channelFactory(paramGetter, 1, pmFactory);
  auto& channels = channelFactory.getChannels();
  BOOST_REQUIRE_EQUAL(channels.size(), 1);
  auto channel = channels[1];
  BOOST_REQUIRE_EQUAL(channel->getID(), 1);
  BOOST_REQUIRE_EQUAL(channel->getThresholdNumber(), 1);
  BOOST_REQUIRE_CLOSE(channel->getThresholdValue(), 80.0, epsilon);
  BOOST_REQUIRE_EQUAL(channel->getPM().getID(), pmFactory.getPMs().at(1)->getID());
}

BOOST_AUTO_TEST_CASE(two_objects)
{
  JPetSetupFactory setupFactory(paramGetter, 2);
  JPetLayerFactory layerFactory(paramGetter, 2, setupFactory);
  JPetSlotFactory slotFactory(paramGetter, 2, layerFactory);
  JPetScinFactory scinFactory(paramGetter, 2, slotFactory);
  JPetPMFactory pmFactory(paramGetter, 2, scinFactory);
  JPetChannelFactory channelFactory(paramGetter, 2, pmFactory);
  auto& channels = channelFactory.getChannels();
  BOOST_REQUIRE_EQUAL(channels.size(), 2);
  auto channel = channels[1];
  BOOST_REQUIRE_EQUAL(channel->getID(), 1);
  BOOST_REQUIRE_EQUAL(channel->getThresholdNumber(), 1);
  BOOST_REQUIRE_CLOSE(channel->getThresholdValue(), 80.0, epsilon);
  BOOST_REQUIRE_EQUAL(channel->getPM().getID(), pmFactory.getPMs().at(1)->getID());
  channel = channels[5];
  BOOST_REQUIRE_EQUAL(channel->getID(), 5);
  BOOST_REQUIRE_EQUAL(channel->getThresholdNumber(), 2);
  BOOST_REQUIRE_CLOSE(channel->getThresholdValue(), 110.0, epsilon);
  BOOST_REQUIRE_EQUAL(channel->getPM().getID(), pmFactory.getPMs().at(1)->getID());
}

BOOST_AUTO_TEST_CASE(missing_field)
{
  JPetSetupFactory setupFactory(paramGetter, 3);
  JPetLayerFactory layerFactory(paramGetter, 3, setupFactory);
  JPetSlotFactory slotFactory(paramGetter, 3, layerFactory);
  JPetScinFactory scinFactory(paramGetter, 3, slotFactory);
  JPetPMFactory pmFactory(paramGetter, 3, scinFactory);
  JPetChannelFactory channelFactory(paramGetter, 3, pmFactory);
  BOOST_REQUIRE_THROW(channelFactory.getChannels(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(wrong_field)
{
  JPetSetupFactory setupFactory(paramGetter, 4);
  JPetLayerFactory layerFactory(paramGetter, 4, setupFactory);
  JPetSlotFactory slotFactory(paramGetter, 4, layerFactory);
  JPetScinFactory scinFactory(paramGetter, 4, slotFactory);
  JPetPMFactory pmFactory(paramGetter, 4, scinFactory);
  JPetChannelFactory channelFactory(paramGetter, 4, pmFactory);
  BOOST_REQUIRE_THROW(channelFactory.getChannels(), std::bad_cast);
}

BOOST_AUTO_TEST_CASE(wrong_relation)
{
  JPetSetupFactory setupFactory(paramGetter, 5);
  JPetLayerFactory layerFactory(paramGetter, 5, setupFactory);
  JPetSlotFactory slotFactory(paramGetter, 5, layerFactory);
  JPetScinFactory scinFactory(paramGetter, 5, slotFactory);
  JPetPMFactory pmFactory(paramGetter, 5, scinFactory);
  JPetChannelFactory channelFactory(paramGetter, 5, pmFactory);
  BOOST_REQUIRE_THROW(channelFactory.getChannels(), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()
