#include "VCVData.h"

#include "VCVDataModel.h"

QVCVDataModel::QVCVDataModel(QObject *parent)
	:QObject(parent)
{

}

QVCVDataModel::~QVCVDataModel()
{
	vector<QVCVData*>::iterator itor;
	for(itor=data_list.begin();itor<data_list.end();itor++)
		delete *itor;

	data_list.clear();
}

void QVCVDataModel::AddData(QVCVData *data)
{
	data_list.push_back(data);
}

bool QVCVDataModel::DeleteData(int index)
{
    if((index<0)||(index>data_list.size()))
        return false;

    QVCVData *data = GetData(index);
    data_list.erase(data_list.begin()+index);

    delete data;
    return true;
}

bool QVCVDataModel::DeleteData(const QString &file_name)
{
	int i;
	for(i=0;i<data_list.size();++i)
	{
		if(data_list[i]->GetFileName()==file_name)
		{
			DeleteData(i);
			return true;
		}
	}

	return false;
}

QVCVData* QVCVDataModel::GetData(int index)
{
		if((index<0)||(index>data_list.size()))
			return NULL;

		return *(data_list.begin()+index);
}

QVCVData* QVCVDataModel::GetData(const QString &file_name)
{
	int i;
	for(i=0;i<data_list.size();++i)
	{
		if(data_list[i]->GetFileName()==file_name)
		{
			return GetData(i);
		}
	}

	return NULL;
}

int QVCVDataModel::Count()
{
	return data_list.size();
}

void QVCVDataModel::Notify()
{
    if(data_list.empty())
        return;
	vector<QVCVData*>::iterator itor;
	for(itor=data_list.begin();itor!=data_list.end();++itor)
	{
		(*itor)->Notify();
	}
}
