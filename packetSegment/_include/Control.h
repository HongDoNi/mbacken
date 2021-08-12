
#include "Segment.h"
#include <string>

class Control : public Segment
{
    
    public:
    
    Control(SegmentEnum segmentID);

    int getFunctionCode() {
        return functionCode;
    }

    void setFunctionCode(int functionCode) {
        this->functionCode = functionCode;
    }

    int getFcv() {
        return fcv;
    }

    void setFcv(int fcv) {
        this->fcv = fcv;
    }

    int getFcbOrAcd() {
        return fcbOrAcd;
    }

    void setFcbOrAcd(int fcbOrAcd) {
        this->fcbOrAcd = fcbOrAcd;
    }

    int getPrm() {
        return prm;
    }

    void setPrm(int prm) {
        this->prm = prm;
    }

    int getDir() {
        return dir;
    }

    void setDir(int dir) {
        this->dir = dir;
    }

    string getAdmin_code() {
        return admin_code;
    }

    void setAdmin_code(string admin_code) {
        this->admin_code = admin_code;
    }

    string getconcentrate_no() {
        return concentrate_no;
    }

    void setconcentrate_no(int concentrate_no) {
        this->concentrate_no = concentrate_no;
    }

    int getIsGroup() {
        return isGroup;
    }

    void setIsGroup(int isGroup) {
        this->isGroup = isGroup;
    }

    int getMsa() {
        return msa;
    }

    void setMsa(int msa) {
        this->msa = msa;
    }

    int getAfn() {
        return afn;
    }

    void setAfn(int afn) {
        this->afn = afn;
    }

    int getSeq() {
        return seq;
    }

    void setSeq(int seq) {
        this->seq = seq;
    }

    int getIsNeedConfirm() {
        return isNeedConfirm;
    }

    void setIsNeedConfirm(int isNeedConfirm) {
        this->isNeedConfirm = isNeedConfirm;
    }

    int getFin() {
        return fin;
    }

    void setFin(int fin) {
        this->fin = fin;
    }

    int getFir() {
        return fir;
    }

    void setFir(int fir) {
        this->fir = fir;
    }

    int getTpV() {
        return tpV;
    }

    void setTpV(int tpV) {
        this->tpV = tpV;
    }

    bool isDecodeCompleted()
    {
        return this->isDecoded;
    }

    void setCodeCompleted()
    {
        this->isDecoded = true; 
    }

    void reset();
   
    private：
    /**
     * 控制域功能码
     */
    int functionCode;
    /**
     * 帧计数有效位
     * <entry key="1" value="fcb位有效"></entry>
     * <entry key="0" value="fcb位无效"></entry>
     */
    int fcv=0;
    /**
     * fcb帧计数位(下行)，acd要求访问位(上行)
     * <entry key="1" value="(下行)启用fcb，(上行)终端有重要事件"></entry>
     * <entry key="0" value="(下行)关闭fcb，(上行)终端无重要事件"></entry>
     */
    int fcbOrAcd;
    /**
     * 启动标志位
     * <entry key="1" value="报文来自启动站"></entry>
     * <entry key="0" value="报文来自从动站"></entry>
     */
    int prm;
    /**
     * 传输方向位
     * <entry key="1" value="上行报文"></entry>
     * <entry key="0" value="下行报文"></entry>
     */
    int dir;
    /**
     * 行政区域码
     *
     */
    int admin_code;
    /**
     * 终端编号
     */
    int concentrate_no;
    /**
     * 是否组地址
     * <entry key="1" value="组地址"></entry>
     * <entry key="0" value="单地址"></entry>
     */
    int isGroup=0;
    /**
     * 主站编码
     */
    int msa;
    /**
     * 功能码
     */
    int afn;
    /**
     * 帧序列
     */
    int seq;
    /**
     * 是否需确认
     * <entry key="1" value="需要确认"></entry>
     * <entry key="0" value="不需要确认"></entry>
     */
    int isNeedConfirm;
    /**
     * 是否报文第一帧
     * <entry key="1" value="报文第一帧"></entry>
     * <entry key="0" value="不是报文第一帧"></entry>
     */
    int fin;
    /**
     * 是否报文最后一帧
     * <entry key="1" value="报文最后一帧"></entry>
     * <entry key="0" value="不是报文最后一帧"></entry>
     */
    int fir;
    /**
     * 附加信息是否有时间标签
     * <entry key="1" value="有时间标签"></entry>
     * <entry key="0" value="无时间标签"></entry>
     */
    int tpV;

    /*
    * 是否完成解析
    */
    bool isDecoded;


    
};
