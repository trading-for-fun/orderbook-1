//
// Created by Chu Ming on 4/11/18.
//

#ifndef PITCH_SPIRIT_BATSAUCTIONUPDATEMSG_HPP
#define PITCH_SPIRIT_BATSAUCTIONUPDATEMSG_HPP

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/spirit/include/qi.hpp>
#include "BATSMessageBase.h"
#include "BATSUtil.h"

namespace qi = boost::spirit::qi;

struct auction_update_wire
{
    std::string symbol;
    char auction_type;
    double reference_price;
    uint32_t buyshares;
    uint32_t sellshares;
    double indicative_price;
    double auction_only_price;
};

BOOST_FUSION_ADAPT_STRUCT( auction_update_wire, symbol, auction_type, reference_price,
                           buyshares, sellshares, indicative_price, auction_only_price )

class BATSAuctionUpdateMsg : public BATSMessageBase
{

public:
    // nested class for decoding the wire msg
    template<typename Iterator>
    struct auction_update_decoder : qi::grammar<Iterator, auction_update_wire()>
    {
        auction_update_decoder(); // default ctor

        qi::rule<Iterator, auction_update_wire()> m_wire_msg; // member variables
        qi::rule<Iterator, double> m_price;
        qi::rule<Iterator, fixed_point() > m_fixed_point;
    };

public:
    BATSAuctionUpdateMsg(int timestamp, char msgtype, std::string const& symbol, char auction_type,
                         double reference_price, uint32_t buyshares, uint32_t sellshares, double indicative_price,
                         double auction_only_price);

    std::string m_symbol;
    char m_auction_type;
    double m_reference_price;
    uint32_t m_buyshares;
    uint32_t m_sellshares;
    double m_indicative_price;
    double m_auction_only_price;
};

BATSAuctionUpdateMsg::BATSAuctionUpdateMsg(int timestamp, char msgtype, std::string const& symbol, char auction_type,
                                           double reference_price, uint32_t buyshares, uint32_t sellshares, double indicative_price,
                                           double auction_only_price) :
        BATSMessageBase(timestamp, msgtype),
        m_symbol(symbol),
        m_auction_type(auction_type),
        m_reference_price(reference_price),
        m_buyshares(buyshares),
        m_sellshares(sellshares),
        m_indicative_price(indicative_price),
        m_auction_only_price(auction_only_price)
{
}

template<typename Iterator>
BATSAuctionUpdateMsg::auction_update_decoder<Iterator>::auction_update_decoder() :
        BATSAuctionUpdateMsg::auction_update_decoder<Iterator>::base_type(m_wire_msg)
{
    // order and execution ids are 12 characters base 36
    qi::uint_parser<uint32_t, 10, 10, 10 > p_shares;
    qi::uint_parser<uint32_t, 10,  6, 6 > int_part;
    qi::uint_parser<uint32_t, 10,  4, 4 > dec_part;

    m_price       = m_fixed_point; // this converts to double from fixed point
    m_fixed_point = int_part >> dec_part;

    m_wire_msg    = qi::repeat(8)[qi::char_]
                    >> qi::char_("OCHI")
                    >> m_price
                    >> p_shares
                    >> p_shares
                    >> m_price
                    >> m_price;

}


#endif //PITCH_SPIRIT_BATSAUCTIONUPDATEMSG_HPP
