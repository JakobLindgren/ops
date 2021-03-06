// TestAll_Subscribe.cpp : Defines the entry point for the console application.
//
#include <windows.h>
#include <ops.h>
#include "TestAll/ChildDataSubscriber.h"
#include "TestAll/BaseDataSubscriber.h"
#include "TestAll/TestAllTypeFactory.h"
#include "TestAll/BaseDataPublisher.h"
#include <iostream>
#include <vector>

class BaseTypeFactory : public ops::SerializableFactory
{
	public:
    ops::Serializable* create(std::string& type)
    {
		if(type == "TestAll.BaseData")
		{
			return new TestAll::BaseData();
		}
		return NULL;

    }

};

//Create a class to act as a listener for OPS data and deadlines
class Main : ops::DataListener, ops::DeadlineMissedListener
{
public:
	ops::Subscriber* baseSub;
	TestAll::BaseDataPublisher* basePub;
	DWORD counter;

public:
	Main(std::string configFile): counter(0)
	{
		using namespace TestAll;
		using namespace ops;

		//Create a topic from configuration.
		ops::Participant* participant = Participant::getInstance("TestAllDomain", "TestAllDomain", configFile);
		participant->addTypeSupport(new BaseTypeFactory());

		ErrorWriter* errorWriter = new ErrorWriter(std::cout);
		participant->addListener(errorWriter);

		Topic topic = participant->createParticipantInfoTopic();

		//Create a BaseSubscriber on that topic.
		baseSub = new Subscriber(topic);
		//baseSub->addFilterQoSPolicy(new KeyFilterQoSPolicy("key1"));
		baseSub->setDeadlineQoS(10000);		
		baseSub->addDataListener(this);
		baseSub->deadlineMissedEvent.addDeadlineMissedListener(this);
		baseSub->start();

		//basePub = new BaseDataPublisher(topic);
		//basePub->setName("BasePublisher");

	}
	///Override from ops::DataListener, called whenever new data arrives.
	void onNewData(ops::DataNotifier* subscriber)
	{
		counter++;

		/*TestAll::BaseData* data;
		data = (TestAll::BaseData*)baseSub->getMessage()->getData();
		if(data == NULL) return;
		std::cout << data->baseText << " " << baseSub->getMessage()->getPublicationID() << " From: " << baseSub->getMessage()->getPublisherName() << std::endl;
		data->setKey("relay");
		basePub->write(data);*/
		std::cout << "Data received! " << counter << std::endl;

		//std::cout << ((ops::ParticipantInfoData*)baseSub->getMessage()->getData())->ips[0] << ":" <<((ops::ParticipantInfoData*)baseSub->getMessage()->getData())->mc_udp_port << std::endl;
	
	
	}
	///Override from ops::DeadlineMissedListener, called if no new data has arrived within deadlineQoS.
	void onDeadlineMissed(ops::DeadlineMissedEvent* evt)
	{
		std::cout << "Deadline Missed!" << std::endl;
	}
	~Main()
	{
	}

};

void newData()
{

}

//Application entry point
int main(int argc, char* args[])
{
	//Add support for our types from TestAll IDL project.
	//ops::OPSObjectFactory::getInstance()->add(new TestAll::TestAllTypeFactory()); 

	std::string configFile = "";
	if (argc > 1) configFile = args[1];
	
	//Create an object that will listen to OPS events
	Main m(configFile);

	//This is a way to create inline subscriber event handlers in c++
	/*class DataCallback : ops::DataListener 
	{ 
		void onNewData(ops::DataNotifier* subscriber)
		{
			
			newData();
		}
	};*/
	//DataCallback callBack;
	


	//Make sure the OPS ioService never runs out of work.
	//Run it on main application thread only.
	while(true)
	{
		Sleep(1000);
		
	}
	return 0;
}

