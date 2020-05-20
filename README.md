## platoon signal threads 
#### author: zhangshuai
truck platoon

### 20200414 
#### author: zhangshuai
* add manual status, if front vehicle (not leader) status is manual, behind vehicle desire mode is abnormal;
* actual mode is manual, desire mode is manual and reset fms order(not leader vehicle),
* abnormal to auto, reset fms order.

### 20200415 
#### author: zhangshuai
* add min formation to keep threshold distance 
* dont't clean plan info, because there has not receive plan info in 300 ms.

## multi threads

### 20200416
#### author: zhangshuai
* complete templateDataContainer.h
* complete receivehighinfo class/ receiveloefreinfo class/ receiveudp class

### 20200506
#### author: zhangshuai
* modify vehicle sequence , when status is cut_in or subleadr, sequence should be 1
* modify Manager::FindLeader, only judge vehicle status, don't judge cut_in flags, 
  when status is cut_in or subleader, is leader
* add multi-hop communication needs information in SendPDM::BroastEgoVehicleInfo3
* fix a logic error in Manager::ProcssCommond abnormal switch, 
  when front dis <= threshold and refuse to keep, shoule goto enqueue.

### 20200507
#### author: zhangshuai
* modify subleader process logic, when ego vehicle cut in is disapper, m_fms_order is enqueue;

### 20200508
#### author: zhangshuai
* fix to fms info error

### 20200511
#### author: zhangshuai
* add licese_map_ in manager.h
* fix Manager::IsAllJoinPlaton, fms platoonmember info include ego vehicle.
* fix receivelowfreinfo.h to clear udpplatoon info when use fms platoonnumber.
* modify send fms vehiclesequence, form ID not from to managerinfo
* add Manager::CalSafeDis and modify **vehicleinfo.cpp**
* add CalSafDis in Manager

### 20200514
#### author: zhangshuai
* modify Manager::IfAbnormal, only check leader and front vehcile connection.

### 20200519 
#### author: zhangshuai
* modify FMS::ChosePlonumber, use fms or hmi lastest platoon number as the final platoon number, when final platoon number is changed, clear platoon_vehicles_map.

### 20200520
#### author: zhangshuai
* modify SendPDM::UpdateInfo, platoon number is from SendDataContanier, origin is switch from hmi or fms, now is both all can use.