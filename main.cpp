#include "mbed.h"
#include "zest-radio-atzbrf233.h"
#include "MQTTNetwork.h"
#include "MQTTmbed.h"
#include "MQTTClient.h"

// Network interface
NetworkInterface net;

int arrivedcount = 0;
const char topic = "/YNOV/messages";

/* Printf the message received and its configuration /
void messageArrived(MQTT::MessageData& md)
{
    MQTT::Message &message = md.message;
    printf("Message arrived: qos %d, retained %d, dup %d, packetid %d\r\n", message.qos, message.retained, message.dup, message.id);
    printf("Payload %.s\r\n", message.payloadlen, (char*)message.payload);
    ++arrivedcount;
}

// MQTT demo
int main() {
    int result;

    // Add the border router DNS to the DNS table
    nsapi_addr_t new_dns = {
        NSAPI_IPv6,
        { 0xfd, 0x9f, 0x59, 0x0a, 0xb1, 0x58, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x01 }
    };
    nsapi_dns_add_server(new_dns);

    printf("Starting MQTT demo\n");

    // Get default Network interface (6LowPAN)
    net = NetworkInterface::get_default_instance();
    if (!net) {
        printf("Error! No network inteface found.\n");
        return 0;
    }

    // Connect 6LowPAN interface
    result = net->connect();
    if (result != 0) {
        printf("Error! net->connect() returned: %d\n", result);
        return result;
    }
// Build the socket that will be used for MQTT
    MQTTNetwork mqttNetwork(net);

    // Declare a MQTT Client
    MQTT::Client<MQTTNetwork, Countdown> client(mqttNetwork);

    // Connect the socket to the MQTT Broker
    const char* hostname = "fd9f:590a:b158:ffff:ffff::c0a8:0103";
    uint16_t port = 1883;
    printf("Connecting to %s:%d\r\n", hostname, port);
    int rc = mqttNetwork.connect(hostname, port);
    if (rc != 0)
        printf("rc from TCP connect is %d\r\n", rc);
 // Send a message with QoS 0
    MQTT::Message message;

    // QoS 0
    char buf[100];
    sprintf(buf, "Soulei - Température : \r\n");

    message.qos = MQTT::QOS0;
    message.retained = false;
    message.dup = false;
    message.payload = (void*)buf;
    message.payloadlen = strlen(buf)+1;

    rc = client.publish(topic, message);

    // yield function is used to refresh the connexion
    // Here we yield until we receive the message we sent
    while (arrivedcount < 1)
        client.yield(100);

    // Disconnect client and socket
    client.disconnect();
    mqttNetwork.disconnect();

    // Bring down the 6LowPAN interface
    net->disconnect();
    printf("Done\n");
}






//Récupérer la température
#include "mbed.h"

namespace {
#define PERIOD_MS 500
}

static DigitalOut led1(LED1);
I2C i2c(I2C1_SDA, I2C1_SCL);
uint8_t lm75_adress = 0x48 << 1;

// main() runs in its own thread in the OS
// (note the calls to Thread::wait below for delays)
int main()
{
    while (true) {
        char cmd[2];
        cmd[0] = 0x00; // adresse registre temperature
        i2c.write(lm75_adress, cmd, 1);
        i2c.read(lm75_adress, cmd, 2);

        float temperature = ((cmd[0] << 8 | cmd[1] ) >> 7) * 0.5;
        printf("Temperature : %f\n", temperature);
        ThisThread::sleep_for(PERIOD_MS);
    }
}
