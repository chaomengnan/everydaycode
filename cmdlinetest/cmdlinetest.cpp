#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/OptionProcessor.h>
#include <Poco/Util/OptionException.h>
#include <iostream>
#include <string>

using namespace Poco::Util;

int main(int argc, char** argv)
{
	OptionSet set;
	set.addOption(
		Option("include-dir", "I", "�ƶ������ļ�")
			.required(false)
			.repeatable(false)
			.argument("path"));

	set.addOption(
		Option("out-file", "o", "����ļ�")
		.required(true)
		.repeatable(false));

	OptionProcessor p(set);

	std::string strName, strValue;
	p.process("-I/usr/include", strName, strValue);
	printf("Name: %s, Value: %s\n", strName.c_str(), strValue.c_str());

	try{
		p.process("-I/usr/include2", strName, strValue);
		printf("Name: %s, Value: %s\n", strName.c_str(), strValue.c_str());
	}
	catch(Poco::Util::DuplicateOptionException& e)
	{
		printf("ѡ���ظ���\n");
	}

	try{
		p.checkRequired();
	}
	catch(Poco::Util::MissingOptionException)
	{
		printf("��δָ����ѡ��\n");
	}

	return 0;
}