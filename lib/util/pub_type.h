#ifndef PUB_TYPE_H
#define PUB_TYPE_H

class ErrCode
{
public:
    enum {
        RET_OK = 0,
        ERR_PUB_GENERAL,
        ERR_PUB_NULLPTR
    };

};

#endif // PUB_TYPE_H
