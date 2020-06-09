bool setSequenceGrouping(INodeMap *nodemap, vector<double>& seqExposureTime, int totalNum = 3)
{
	if (NULL == nodemap)
		return false;

	CBooleanPtr sequence_Enable(nodemap->GetNode("SequenceEnable"));
	if (sequence_Enable && IsWritable(sequence_Enable))
	{
	
		if (sequence_Enable->GetValue() == true)
			sequence_Enable->SetValue(false);
		assert(sequence_Enable->GetValue() == false);

		CIntegerPtr sequenceTotalNum(nodemap->GetNode("SequenceSetTotalNumber"));
		CFloatPtr sequenceSetExposureTime(nodemap->GetNode("ExposureTimeAbs"));
		// SequenceSetStore(nodemap->GetNode("SequenceSetStore"));
		if(sequenceTotalNum && IsWritable(sequenceTotalNum))
		{
			assert(totalNum > 0 && seqExposureTime.size() >= totalNum);
			sequenceTotalNum->SetValue(totalNum);
			assert(sequenceTotalNum->GetValue(totalNum));
			CIntegerPtr sequenceSetIndex(nodemap->GetNode("SequenceSetIndex"));
			if (sequenceSetIndex && IsWritable(sequenceSetIndex))
			{
				for (int i = 0; i < totalNum; i++)
				{
					sequenceSetIndex->SetValue(i);
					assert(sequenceSetIndex->GetValue() == i);
					if (sequenceSetExposureTime && IsWritable(sequenceSetExposureTime))
					{
						sequenceSetExposureTime->SetValue(seqExposureTime[i]);
						assert(sequenceSetExposureTime->GetValue() == seqExposureTime[i]);

					}
				
				}
			}
		}
	}
	else
		return false;
	if (IsWritable(sequence_Enable))
	{
		sequence_Enable->SetValue(true);
		assert(sequence_Enable->GetValue() == true);
	}
	return true;
}