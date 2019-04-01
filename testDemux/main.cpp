
#include <iostream>
extern "C" {
	#include "libavformat/avformat.h"
}
using namespace std;
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avutil.lib")
static double r2d(AVRational r)
{
	return r.den == 0 ? 0 : (double)r.num / (double)r.den;
}

int main(int argc, char *argv[])
{
	cout << "Test Demux FFmpeg.club" << endl;
	const char *path = "video.mp4";
	//��ʼ����װ��
	av_register_all();

	//��ʼ������� �����Դ�rtsp rtmp http Э�����ý����Ƶ��
	avformat_network_init();

	//��������
	AVDictionary *opts = NULL;
	//����rtsp����tcpЭ���
	av_dict_set(&opts, "rtsp_transport", "tcp", 0);

	//������ʱʱ��
	av_dict_set(&opts, "max_delay", "500", 0);


	//���װ������
	AVFormatContext *ic = NULL;
	int re = avformat_open_input(
		&ic,
		path,
		0,  // 0��ʾ�Զ�ѡ������
		&opts //�������ã�����rtsp����ʱʱ��
	);
	if (re != 0)
	{
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf) - 1);
		cout << "open " << path << " failed! :" << buf << endl;
		getchar();
		return -1;
	}
	cout << "open " << path << " success! " << endl;

	//��ȡ����Ϣ 
	re = avformat_find_stream_info(ic, 0);

	//��ʱ�� ����
	int totalMs = ic->duration / (AV_TIME_BASE / 1000);
	cout << "totalMs = " << totalMs << endl;

	//��ӡ��Ƶ����ϸ��Ϣ
	av_dump_format(ic, 0, path, 0);

	//����Ƶ��������ȡʱ��������Ƶ
	int videoStream = 0;
	int audioStream = 1;

	//�ٻ�ȡ����Ƶ����Ϣ ��������������ȡ��
	for (int i = 0; i < ic->nb_streams; i++)
	{
		AVStream *as = ic->streams[i];
		cout << "codec_id = " << as->codecpar->codec_id << endl;
		cout << "format = " << as->codecpar->format << endl;

		//��Ƶ AVMEDIA_TYPE_AUDIO
		if (as->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audioStream = i;
			cout << i << "��Ƶ��Ϣ" << endl;
			cout << "sample_rate = " << as->codecpar->sample_rate << endl;
			//AVSampleFormat;
			cout << "channels = " << as->codecpar->channels << endl;
			//һ֡���ݣ��� ��ͨ�������� 
			cout << "frame_size = " << as->codecpar->frame_size << endl;
			//1024 * 2 * 2 = 4096  fps = sample_rate/frame_size

		}
		//��Ƶ AVMEDIA_TYPE_VIDEO
		else if (as->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStream = i;
			cout << i << "��Ƶ��Ϣ" << endl;
			cout << "width=" << as->codecpar->width << endl;
			cout << "height=" << as->codecpar->height << endl;
			//֡�� fps ����ת��
			cout << "video fps = " << r2d(as->avg_frame_rate) << endl;
		}
	}

	//�ڻ�ȡ��Ƶ�� ������ȡ
	videoStream = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	///ic->streams[videoStream]


	if (ic)
	{
		//�ͷŷ�װ�����ģ����Ұ�ic��0
		avformat_close_input(&ic);
	}

	getchar();
	return 0;
}