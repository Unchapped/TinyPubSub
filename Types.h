#ifndef TinyPubSub_Types_h
#define TinyPubSub_Types_h

#include <Arduino.h>


typedef unsigned char TopicId;
typedef unsigned char MsgType;

//System topics and callbacks
//Notifications of new publishers
#define TOPIC_NOTIFY 0x00
//Notifications of new subscription requests
#define TOPIC_REQUEST 0x01
//error messages and 
#define TOPIC_DEBUG 0x02
//generic console chatter
#define TOPIC_CHATTER 0x03

//topic info is a tuple of topic ID and Message type, used for subscriber requests, and publisher notifications
#define TYPE_TOPICINFO 0x00
#define LEN_TOPICINFO (sizeof(TopicId) + sizeof(MsgType))


#define TYPE_EMPTY 0x01
#define LEN_EMPTY 0

#define TYPE_CHAR 0x02
#define TYPE_CHARARRAY 0x12


#define TYPE_INT 0x03
#define TYPE_LONG 0x04
#define TYPE_FLOAT 0x05

#define TYPE_CHARARRAY 0x03



struct TPSMessage {
	TPSMessage(MsgType type, size_t length = 0); //for general message types, length is cased by type
	MsgType type;
	size_t length;
	char[] data;
};

class TPSTransport {
	friend class TPSNode;
	TPSNode &_parent;
	virtual int init(TPSNode &node = TinyPubSub);
protected:
	virtual bool hasSubscribers(unsigned char topic_id);
	virtual bool poll(); //used to check for incoming streams...
	virtual transmit_cb(); //TODO: figure out how to make this work, when transports recieve subscriber notification
};

//send messages over any genral Arduino Stream
class StreamTransport : TPSTransport {
	void init(Stream& stream);
	void poll(); //send recieve queued messages over this connection
};


class TPSNode {
	friend class TPSTransport;
	struct _msgTuple{
		char is_dirty;
		TPSMessage 
	};
	vector<TPSTransport*> _transports;
	vector

protected:
	virtual bool hasSubscribers(unsigned char topic_id);

public:
	void init();
	void subscribe(TopicId topic_id, MsgType msg_type, void (*callbackFunction)(TPSMessage&));
	void notify(TopicId id, MsgType type); //tells all remote nodes what topics we publish
	void publish(TopicId id, TPSMessage& message, bool source_local = true); //local messages get passed on to registered transports, remote ones do not.
	void registerTransport(TPSTransport& transport);
	void poll();
};

extern TPSNode TinyPubSub;

#endif