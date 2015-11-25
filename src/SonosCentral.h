/* Copyright 2013-2015 Sathya Laufer
 *
 * Homegear is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Homegear is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Homegear.  If not, see <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */

#ifndef SONOSCENTRAL_H_
#define SONOSCENTRAL_H_

#include "homegear-base/BaseLib.h"
#include "SonosPeer.h"

#include <memory>
#include <mutex>
#include <string>

namespace Sonos
{

class SonosCentral : public BaseLib::Systems::ICentral
{
public:
	SonosCentral(ICentralEventSink* eventHandler);
	SonosCentral(uint32_t deviceType, std::string serialNumber, ICentralEventSink* eventHandler);
	virtual ~SonosCentral();
	virtual void dispose(bool wait = true);

	virtual bool onPacketReceived(std::string& senderID, std::shared_ptr<BaseLib::Systems::Packet> packet);
	std::string handleCliCommand(std::string command);
	uint64_t getPeerIdFromSerial(std::string serialNumber) { std::shared_ptr<SonosPeer> peer = getPeer(serialNumber); if(peer) return peer->getID(); else return 0; }

	std::shared_ptr<SonosPeer> getPeer(uint64_t id);
	std::shared_ptr<SonosPeer> getPeer(std::string serialNumber);
	virtual void loadPeers();
	virtual void savePeers(bool full);
	virtual void loadVariables() {}
	virtual void saveVariables() {}

	virtual void homegearShuttingDown();

	virtual PVariable deleteDevice(int32_t clientID, std::string serialNumber, int32_t flags);
	virtual PVariable deleteDevice(int32_t clientID, uint64_t peerID, int32_t flags);
	virtual PVariable getDeviceInfo(int32_t clientID, uint64_t id, std::map<std::string, bool> fields);
	virtual PVariable putParamset(int32_t clientID, std::string serialNumber, int32_t channel, ParameterGroup::Type::Enum type, std::string remoteSerialNumber, int32_t remoteChannel, PVariable paramset);
	virtual PVariable putParamset(int32_t clientID, uint64_t peerID, int32_t channel, ParameterGroup::Type::Enum type, uint64_t remoteID, int32_t remoteChannel, PVariable paramset);
	virtual PVariable searchDevices(int32_t clientID);
protected:
	std::unique_ptr<BaseLib::SSDP> _ssdp;
	bool _shuttingDown = false;

	bool _stopWorkerThread = false;
	std::thread _workerThread;

	std::shared_ptr<SonosPeer> createPeer(BaseLib::Systems::LogicalDeviceType deviceType, std::string serialNumber, std::string ip, std::string softwareVersion, std::string idString, std::string typeString, bool save = true);
	void deletePeer(uint64_t id);
	virtual void worker();
	virtual void init();
};

}

#endif