#include <list>
#include <map>
#include <string>

#include "Packet.h"
#include "BinPacket.h"
#include "CodeConfig.h"

class Protocol 
{
    /**
     * 规约解码
     * 返回值0，解码失败，抛出异常
     * 返回值1，解码成功
     * 返回值2，帧序号不一致
     * 返回值4，返回帧有时间标识，并且该报文已超时
     * 返回值8，需等待后续帧
     * 返回值16,需要回复确认帧
     * 返回值32,主动上传帧
     * 返回值64，有事件上报
     * 返回值128，该报文是广播报文
     *
     */
    virtual int decode(unsigned char in[], int nLen,  Packet out)=0;

    /**
     * 规约编码
     * @param in
     * @param out
     * @return
     * 返回值0，编码失败，抛出异常
     * 返回值1，编码成功
     * 返回值2，需等待确认帧
     * 返回值4, 需要等待回复
     */
    virtual int encode(Packet in, unsigned char* pout)=0;

    /**
     * 获取解码配置
     * @return
     */

    virtual CodeConfig getCodeConfig()=0;

    /**
     * 获取字段的描述
     * @param field 字段名
     * @return 字段的中文描述
     */
    virtual string getFieldRemark(string command, string field)=0;

    /**
     * 获取字段的单位
     * @param field 字段名
     * @return 字段的单位
     */
    virtual string getFieldUnit(string command, string field)=0;

    /**
     * 获取数据的描述
     * @param field 字段名
     * @param value 字段值
     * @return 数据的中文描述
     */
    virtual string getValueDescription(string command, string field, int value)=0;

    /**
     * 获取字段的可枚举列表
     * @param field 字段名
     * @return 枚举列表
     */
    virtual map<int, string> getValueDescriptionList(string command, string field)=0;

    /**
     * 获取支持数据项列表
     * @return 所有数据项名称
     */
    virtual list<string> getDataList()=0;

    /**
     * 获取支持事件列表
     * @return
     */
    virtual list<string> getEventList()=0;

    /**
     * 获取数据项的输入流模板
     * @param command 数据项名称
     * @return 输入流模板
     */
    virtual string getDataTemplate(string command)=0;
};