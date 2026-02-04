

/*
PROBLEM:
Design a simple Pub/Sub system:
- Topics exist (Sports, News, etc.)
- Subscribers subscribe/unsubscribe to topics
- Publishers publish messages to a topic
- All subscribers of that topic are notified

PATTERN:
Observer Pattern
- Topic = Subject
- Subscriber = Observer
- Broker = Topic registry (decouples publisher from topics)

WHY:
- One-to-many notification
- Loose coupling between publisher and subscribers
- Easy to add new subscribers

TRADE-OFFS:
+ Simple, flexible, decoupled
- In-memory only
- No persistence or delivery guarantee
- Not thread-safe

USE WHEN:
- Event/notification systems
- UI listeners
- Logging/monitoring
- Simple Pub/Sub logic

AVOID WHEN:
- Need message durability
- Distributed systems (Kafka/RabbitMQ)
- High scale or reliability needed
*/



#include<iostream>
#include<unordered_map>
#include<unordered_set>
using namespace std;

class Subscriber;
class Topic;

class Subscriber {
    string subscriberName;

public:
    Subscriber(string name) : subscriberName(name) {}

    void notify(const string& topicName, const string& msg) {
        cout << "[NOTIFY] " << subscriberName
             << " received on [" << topicName << "]: "
             << msg << endl;
    }

    string getName() const {
        return subscriberName;
    }
};

class Topic {
private:
    string topicName;
    unordered_set<Subscriber*> subscribers;

public:
    Topic(const string& name) : topicName(name) {}

    void subscribe(Subscriber* subscriber) {
        if (subscribers.find(subscriber) != subscribers.end())
            cout << "[INFO] " << subscriber->getName()
                 << " already subscribed to " << topicName << endl;
        else {
            subscribers.insert(subscriber);
            cout << "[SUBSCRIBE] " << subscriber->getName()
                 << " subscribed to " << topicName << endl;
        }
    }

    void unSubscribe(Subscriber* subscriber) {
        if (subscribers.find(subscriber) == subscribers.end())
            cout << "[INFO] " << subscriber->getName()
                 << " is not subscribed to " << topicName << endl;
        else {
            subscribers.erase(subscriber);
            cout << "[UNSUBSCRIBE] " << subscriber->getName()
                 << " unsubscribed from " << topicName << endl;
        }
    }

    void notify(const string& msg) {
        cout << "\n[PUBLISH] Message on topic: " << topicName << endl;
        for (auto subscriber : subscribers)
            subscriber->notify(topicName, msg);
    }
};

class Broker {
private:
    unordered_map<string, Topic*> topics;

public:
    Topic* createTopic(const string& topicName) {
        if (topics.find(topicName) != topics.end()) {
            cout << "[INFO] Topic already exists: " << topicName << endl;
            return topics[topicName];
        }
        Topic* newTopic = new Topic(topicName);
        topics[topicName] = newTopic;
        cout << "[BROKER] Created topic: " << topicName << endl;
        return newTopic;
    }

    Topic* getTopic(const string& name) {
        if (topics.find(name) == topics.end()) {
            cout << "[ERROR] No topic named: " << name << endl;
            return nullptr;
        }
        return topics[name];
    }
};

class Publisher {
    Broker* broker;
    string publisherName;

public:
    Publisher(string name, Broker* broker)
        : publisherName(name), broker(broker) {}

    void publishMessage(const string& topic, const string& msg) {
        cout << "\n[PUBLISHER] " << publisherName
             << " publishing to " << topic << endl;

        Topic* topicObj = broker->getTopic(topic);
        if (!topicObj)
            cout << "[FAILED] Topic does not exist: " << topic << endl;
        else
            topicObj->notify(msg);
    }
};

int main() {
    cout << "==== PUB-SUB SYSTEM DEMO ====\n\n";

    Broker* broker = new Broker();

    // Create Topics
    Topic* sportsTopic = broker->createTopic("Sports");
    Topic* newsTopic = broker->createTopic("News");
    Topic* entertainmentTopic = broker->createTopic("Entertainment");

    cout << endl;

    // Create Subscribers
    Subscriber* aditya = new Subscriber("Aditya");
    Subscriber* yash = new Subscriber("Yash");
    Subscriber* rohan = new Subscriber("Rohan");

    // Create Publishers
    Publisher* sportsPublisher = new Publisher("SportsPublisher", broker);
    Publisher* entertainmentPublisher = new Publisher("EntertainmentPublisher", broker);
    Publisher* newsPublisher = new Publisher("NewsPublisher", broker);

    cout << "\n==== SUBSCRIPTIONS ====\n";
    sportsTopic->subscribe(aditya);
    sportsTopic->subscribe(yash);
    sportsTopic->subscribe(yash); // duplicate test

    newsTopic->subscribe(yash);
    newsTopic->subscribe(rohan);

    entertainmentTopic->subscribe(aditya);

    cout << "\n==== FIRST ROUND OF PUBLISHING ====\n";
    sportsPublisher->publishMessage("Sports", "India won 2027 World Cup!");
    entertainmentPublisher->publishMessage("Entertainment", "Dhurandhar-2 releases March 19!");
    newsPublisher->publishMessage("News", "America reduced tariff to 18% on India.");

    cout << "\n==== UNSUBSCRIBE TEST ====\n";
    sportsTopic->unSubscribe(yash);
    sportsTopic->unSubscribe(yash); // double unsubscribe

    cout << "\n==== SECOND ROUND OF PUBLISHING ====\n";
    sportsPublisher->publishMessage("Sports", "CSK won IPL 2026!");
    newsPublisher->publishMessage("News", "Sensex hits all-time high.");

    cout << "\n==== INVALID TOPIC TEST ====\n";
    sportsPublisher->publishMessage("Politics", "New bill passed.");

    cout << "\n==== END OF DEMO ====\n";
    return 0;
}