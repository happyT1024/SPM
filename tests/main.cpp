#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <SIPParser/SIP_Message.h>

int main(int argc, char **argv){
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::InitGoogleMock(&argc, argv);

  return RUN_ALL_TESTS();
}

TEST(SIP_Message_Parse,ParseFrom_test){
  std::string str = "From: Alice <sip:alice@example.com>;tag=1928301774";
  Ds_SIP_Message::Ds_from from;
  from.parse(str);
  ASSERT_EQ(from.tag, "1928301774");
  ASSERT_EQ(from.name_address.to_str(), "Alice <sip:alice@example.com>");
  ASSERT_EQ(from.name_address.display_name, "Alice");
  ASSERT_EQ(from.name_address.uri, "<sip:alice@example.com>");
  ASSERT_EQ(from.to_str(), str+"\r\n");
}

TEST(SIP_Message_Parse,ParseTo_test){
  std::string str = "To: Alice <sip:alice@example.com>";
  Ds_SIP_Message::Ds_to to;
  to.parse(str);
  ASSERT_EQ(to.tag, "");
  ASSERT_EQ(to.name_address.to_str(), "Alice <sip:alice@example.com>");
  ASSERT_EQ(to.name_address.display_name, "Alice");
  ASSERT_EQ(to.name_address.uri, "<sip:alice@example.com>");
  ASSERT_EQ(to.to_str(), str+"\r\n");
}

TEST(SIP_Message_Parse, ParseMsg_test)
{
  std::string str =
      "INVITE sip:bob@example.com SIP/2.0\r\n"
      "Via: SIP/2.0/UDP pc33.example.com;branch=z9hG4bK776asdhds\r\n"
      "From: Alice <sip:alice@example.com>;tag=1928301774\r\n"
      "To: Bob <sip:bob@example.com>\r\n"
      "Call-ID: a84b4c76e66710@pc33.example.com\r\n"
      "Max-Forwards: 70\r\n"
      "Contact: <sip:alice@pc33.example.com>\r\n"
      "Content-Type: application/sdp\r\n"
      "Content-Length: 142\r\n"
      "\r\n"
      "(SDP content would be here)";
  Ds_SIP_Message sipMessage(str);
  ASSERT_EQ(sipMessage.begin, "INVITE sip:bob@example.com SIP/2.0\r\n"
                              "Via: SIP/2.0/UDP pc33.example.com;branch=z9hG4bK776asdhds\r\n");
  ASSERT_EQ(sipMessage.from.to_str(), "From: Alice <sip:alice@example.com>;tag=1928301774\r\n");
  ASSERT_EQ(sipMessage.to.to_str(), "To: Bob <sip:bob@example.com>\r\n");
  ASSERT_EQ(sipMessage.end,
                            "Call-ID: a84b4c76e66710@pc33.example.com\r\n"
                            "Max-Forwards: 70\r\n"
                            "Contact: <sip:alice@pc33.example.com>\r\n"
                            "Content-Type: application/sdp\r\n"
                            "Content-Length: 142\r\n"
                            "\r\n"
                            "(SDP content would be here)");
}
