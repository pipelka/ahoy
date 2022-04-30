#ifndef __HM_SYSTEM_H__
#define __HM_SYSTEM_H__

#include "hmInverter.h"
#ifndef NO_RADIO
#include "hmRadio.h"
#endif



template <class RADIO, class BUFFER, uint8_t MAX_INVERTER=3, class INVERTERTYPE=Inverter<float>>
class HmSystem {
    public:
        typedef RADIO RadioType;
        RadioType Radio;
        typedef BUFFER BufferType;
        BufferType BufCtrl;

        HmSystem() {
            mNumInv = 0;
        }
        ~HmSystem() {
            // TODO: cleanup
        }

        void setup() {
            Radio.setup(&BufCtrl);
        }

        INVERTERTYPE *addInverter(const char *name, uint64_t serial, uint8_t type) {
            if(MAX_INVERTER <= mNumInv) {
                DPRINT("max number of inverters reached!");
                return NULL;
            }
            INVERTERTYPE *p = &mInverter[mNumInv];
            p->id         = mNumInv;
            p->serial.u64 = serial;
            p->type       = type;
            p->init();
            uint8_t len   = (uint8_t)strlen(name);
            strncpy(p->name, name, (len > MAX_NAME_LENGTH) ? MAX_NAME_LENGTH : len);

            if(NULL == p->assign) {
                DPRINT("no assignment for type found!");
                return NULL;
            }
            else {
                mNumInv ++;
                return p;
            }
        }

        INVERTERTYPE *findInverter(uint8_t buf[]) {
            INVERTERTYPE *p;
            for(uint8_t i = 0; i < mNumInv; i++) {
                p = &mInverter[i];
                if((p->serial.b[3] == buf[0])
                    && (p->serial.b[2] == buf[1])
                    && (p->serial.b[1] == buf[2])
                    && (p->serial.b[0] == buf[3]))
                    return p;
            }
            return NULL;
        }

        INVERTERTYPE *getInverterByPos(uint8_t pos) {
            if(mInverter[pos].serial.u64 != 0ULL)
                return &mInverter[pos];
            else
                return NULL;
        }

        uint8_t getNumInverters(void) {
            return mNumInv;
        }

    private:
        INVERTERTYPE mInverter[MAX_INVERTER];
        uint8_t mNumInv;
};

#endif /*__HM_SYSTEM_H__*/