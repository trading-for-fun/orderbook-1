
#ifndef ALGOSEEK_ORDEREXECUTEDMSG_HPP
#define ALGOSEEK_ORDEREXECUTEDMSG_HPP

#include <ctime>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include "MessageBase.h"

namespace qi = boost::spirit::qi;
namespace phi = boost::phoenix;

namespace algoseek
{

    class OrderExecutedMsg : public OrderMsgBase
    {

    public:
        template <typename Iterator>
        struct order_executed_decoder : decoder_base, qi::grammar<Iterator, OrderExecutedMsg()>
        {
            order_executed_decoder(timespec timestamp, char msgtype, int8_t side, int64_t order_id);

            qi::rule<Iterator, OrderExecutedMsg()> m_wire_msg; // member variables
        };

    public:

        OrderExecutedMsg() : OrderMsgBase() {}

        OrderExecutedMsg(timespec timestamp, char msgtype, int64_t orderId, uint32_t shares,
                    uint64_t price, int8_t side) : OrderMsgBase(timestamp, msgtype,
                                                                orderId, shares,
                                                                price, side)
                    
        {}

    };

    template<typename Iterator>
    OrderExecutedMsg::order_executed_decoder<Iterator>::order_executed_decoder(timespec timestamp, char msgtype, int8_t side, int64_t order_id) :
        decoder_base(timestamp, msgtype, side, order_id),
        OrderExecutedMsg::order_executed_decoder<Iterator>::base_type(m_wire_msg)
    {
        m_wire_msg = ( qi::as_string[ +qi::alnum ] >> "," >>
                       qi::float_                  >> "," >>
                       qi::uint_                   >> "," >>
                       qi::as_string[ *qi::alpha ] >> "," >>  
                       qi::as_string[ *qi::alpha ] )  
                [qi::_val = phi::construct<OrderExecutedMsg> (
                        m_ts, m_mtype, m_orderId, qi::_3, qi::_2 * PRICE_MULTIPLIER, m_side )]; 
    }

} // namespace algoseek

#endif //ALGOSEEK_ORDEREXECUTEDMSG_HPP