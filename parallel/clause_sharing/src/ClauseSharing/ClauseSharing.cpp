//
// Created by Matteo on 02/12/15.
//

#include "ClauseSharing.h"


redisContext *ClauseSharing::Connect(const char *hostname, int port) {
    redisContext *context;
    redisReply *reply;
    struct timeval timeout = {1, 500000}; // 1.5 seconds

    /* CONNECT */
    context = redisConnectWithTimeout(hostname, port, timeout);
    if (context == NULL || context->err) {
        if (context) {
            redisFree(context);
            throw "Connection error"; // << context->errstr << "\n";
        } else {
            throw "Connection error: can't allocate redis context";
        }
    }

    /* PING */
    reply = (redisReply *) redisCommand(context, "PING");
    if (reply == NULL) {
        throw "PING to the server failed";
    }
    freeReplyObject(reply);

    return context;
}

ClauseSharing::ClauseSharing(char *channel, char *hostname, uint16_t port) : Thread() {
    this->channel = std::string(channel);
    this->context_pub = ClauseSharing::Connect(hostname, port);
    this->context_sub = ClauseSharing::Connect(hostname, port);
    redisReply *reply = (redisReply *) redisCommand(context_pub, "DEL %s", this->channel.c_str());
    freeReplyObject(reply);
}

ClauseSharing::~ClauseSharing() {
    redisFree(this->context_pub);
    redisFree(this->context_sub);
}

void ClauseSharing::thread_main() {
    try {
        redisReply *reply;
        reply = (redisReply *) redisCommand(context_sub, "SUBSCRIBE %s.out", this->channel.c_str());
        freeReplyObject(reply);
        while (redisGetReply(context_sub, (void **) &reply) == REDIS_OK) {
            assert (reply->type == REDIS_REPLY_ARRAY && reply->elements == 3);
            assert (std::string(reply->element[0]->str).compare("message") == 0);

            channel = std::string(reply->element[1]->str);
            std::cout << "message on channel: " << channel << "\n";

            Message m;
            std::string s = std::string(reply->element[2]->str, (size_t) reply->element[2]->len);
            m.load(s);
            freeReplyObject(reply);

            uint32_t o = 0;
            while (o < m.payload.length()) {
                vec<Lit> lits;
                clauseDeserialize(m.payload, &o, lits);
                if (lits.size() <= 0 || lits.size() > 50) {
                    continue;
                }
                sort(lits);
                std::string str;
                clauseSerialize(lits, str);
                //ZADD clauses NX %d %b
                reply = (redisReply *) redisCommand(context_pub, "SADD clauses %b", str.c_str(), str.size());
                //if(reply->integer==0)
                //  std::cout << '!';
                freeReplyObject(reply);
            }
        }
    }
    catch (Thread::StopException e) { }
}
