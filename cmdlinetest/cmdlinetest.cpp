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
		Option("include-dir", "I", "制定包含文件")
			.required(false)
			.repeatable(false)
			.argument("path"));

	set.addOption(
		Option("out-file", "o", "输出文件")
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
		printf("选项重复了\n");
	}

	try{
		p.checkRequired();
	}
	catch(Poco::Util::MissingOptionException)
	{
		printf("有未指定的选项\n");
	}

	return 0;
}