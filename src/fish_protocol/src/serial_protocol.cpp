/**
 * @brief 文件描述：待完善
 * @author 小鱼 (fishros@foxmail.com)
 * @version V1.0.0
 * @date 2022-07-24
 * @copyright 版权所有：（鱼香ROS）fishros.org.cn
 */
#include "fish_protocol/serial_protocol.h"


namespace serial_protocol {


/// @brief Flush a serial port's buffers.
///
/// @param serial_port Port to flush.
/// @param what Determines the buffers to flush.
/// @param error Set to indicate what error occurred, if any.
void SerialProtocol::flush_serial_port(
        boost::asio::serial_port& serial_port,
        flush_type what,
        boost::system::error_code& error)
{
    if (0 == ::tcflush(serial_port.lowest_layer().native_handle(), what))
    {
        error = boost::system::error_code();
    }
    else
    {
        error = boost::system::error_code(errno,
            boost::asio::error::get_system_category());
    }
}

SerialProtocol::~SerialProtocol()
{
    io_service_.stop();
    serial_port_.cancel();
    serial_port_.close();
}
void SerialProtocol::_initSerialProtocol() {
    boost::system::error_code ec;
    try
    {
        serial_port_.open(protocol_config_.serial_address_, ec);
        if(!serial_port_.is_open())
            throw ec;
    }
    catch(boost::system::error_code& ec)
    {
        std::cout << ec.message() << std::endl;
        return ;
    }
    std::cout << "串口已打开:" << protocol_config_.serial_address_ << std::endl;
    serial_port_.set_option(spb::baud_rate(protocol_config_.serial_baut_));
    serial_port_.set_option(spb::character_size(8));
    serial_port_.set_option(spb::stop_bits(spb::stop_bits::one));
    serial_port_.set_option(spb::parity(spb::parity::none));
    serial_port_.set_option(spb::flow_control(spb::flow_control::none));
    boost::thread td(boost::bind(&boost::asio::io_service::run, &io_service_));
    _asyncReadSomeData();
}

void SerialProtocol::_recvDataCallback(const boost::system::error_code& error,
                                        size_t bytes_transferred) {
    if (bytes_transferred > 0) {
        // 回调数据
        if(recv_uint8_callback)
        {
            recv_uint8_callback(recv_data_buffer_, bytes_transferred);
        }
    }
    _asyncReadSomeData();
}

void SerialProtocol::_asyncReadSomeData() {
    serial_port_.async_read_some(
        boost::asio::buffer(recv_data_buffer_, sizeof(recv_data_buffer_)),
        boost::bind(&SerialProtocol::_recvDataCallback, this,
                    boost::placeholders::_1, boost::placeholders::_2));

}

int SerialProtocol::ProtocolSendString(const std::string& data) {
    serial_port_.write_some(boost::asio::buffer(data.data(), data.size()));
    return 0;
}
int SerialProtocol::ProtocolSenduint8_t(const std::uint8_t* data, const std::uint8_t len) {
    serial_port_.write_some(boost::asio::buffer(data, len));
    return 0;
}

void SerialProtocol::SetDataRecvCallback(
        std::function<void(const std::uint8_t*, const std::uint8_t)> callback) {
    recv_uint8_callback = callback;
}


}  // namespace fish_protocol
