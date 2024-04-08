#pragma once

#include "DasherInput.h"
#include "asio.hpp"
#include "DashIntfSettings.h"
#include "PressFilter.h"

class SocketInput : public Dasher::CScreenCoordInput, public Dasher::CDefaultFilter
{
public:
    SocketInput(CSettingsUser* Creator, Dasher::CDashIntfSettings* Controller, Dasher::CFrameRate* pFramerate);
    virtual ~SocketInput();

	bool GetScreenCoords(Dasher::screenint& iX, Dasher::screenint& iY, Dasher::CDasherView* pView) override;

    void startListen();
    void stopListen();

    bool GetSettings(SModuleSettings** sets, int* iCount) override;
private:
    bool relativeMode;
	double lastRelativeX = 0.5;
	double lastRelativeY = 0.5;
    Dasher::screenint lastX = 0;
    Dasher::screenint lastY = 0;

    //Dasher Stuff
    Dasher::CDashIntfSettings* Controller;

    //Commands
    std::string xLabel;
    std::string yLabel;
    std::string startStopLabel = "s";

    // ASIO Stuff
    asio::io_service io_service;
    std::unique_ptr<asio::ip::tcp::acceptor> session_acceptor;
    std::unique_ptr<asio::ip::tcp::socket> open_socket;
    asio::streambuf stream_buffer;

    // Async Thread
    std::thread receiveThread;
    bool keepReadThreadAlive = true;
    bool keepListenServerAlive = true;
};