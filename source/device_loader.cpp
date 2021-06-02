#include "device_loader.hpp"
#include "fire_sensor_creator.hpp"
#include "sprinkler_controller_creator.hpp"
#include <dlfcn.h>

namespace smarthome
{

DeviceLoader::DeviceLoader(ICreatorsRegistrator& a_register)
:   m_register(a_register)
{
}

void DeviceLoader::Upload(std::string a_deviceType)
{   
    // if(!a_deviceType.compare("FireSensor"))
    // {
    //     FireSensorRegistration(m_register);
    // }  
    // else if(!a_deviceType.compare("SprinklerController"))
    // {
    //     SprinklerControllerRegistration(m_register);
    // }
    // else 
    // {
    //     std::cout << "yotam gay" << std::endl;
    // }
    
    std::string path = "../../source/";
    if(!a_deviceType.compare("SprinklerController"))
    {
        path+="sprinkler_controller_creator.so"; 
    }
    else if(!a_deviceType.compare("FireSensor"))
    {
        path+="fire_sensor_creator.so";
    }
    
    //a_deviceType=
    std::cout<<"before dlopen\n";
    void* handle = dlopen(path.c_str(), RTLD_NOW);
    if(!handle)
    {
        std::cout<<dlerror()<<std::endl;
    }
    std::cout<<"after dlopen\n";
    void (*pf)(ICreatorsRegistrator & );
    path.clear();
    path+=a_deviceType+="Registration";
    pf = (void (*)(ICreatorsRegistrator & ))dlsym(handle,path.c_str());
    if(!pf)
    {
        std::cout<<dlerror()<<std::endl;
    }
    
    pf(m_register);     
    
}


} // smarthome