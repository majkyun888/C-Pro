bool setBinning(INodeMap &nodemap, int Horizontal, int Vertical)
{
	/*if (NULL == nodemap)
		return false;*/
	assert(Horizontal > 0 && Horizontal <= 4 && Vertical > 0 && Vertical <= 4);

	CBooleanPtr sequence_Enable(nodemap.GetNode("SequenceEnable"));
	CBooleanPtr sequence_Enableq(nodemap.GetNode("SequenceSetStore"));
	 //SequenceAdvanceModeEnums
	if (sequence_Enable && IsWritable(sequence_Enable))
	{
	
		if (sequence_Enable->GetValue() == true)
			sequence_Enable->SetValue(false);
		assert(sequence_Enable->GetValue() == false);

		CIntegerPtr bHorizontal(nodemap.GetNode("BinningHorizontal"));
		if (bHorizontal && IsWritable(bHorizontal))
		{
			bHorizontal->SetValue(Horizontal);
			assert(bHorizontal->GetValue() == Horizontal);
		}
		CIntegerPtr bVertical(nodemap.GetNode("BinningVertical"));
		if (bVertical && IsWritable(bVertical))
		{
			bVertical->SetValue(Vertical);
			assert(bVertical->GetValue() == Vertical);
		}
	}
	else
		return false;

	return true;
}
// Number of images to be grabbed.
static const uint32_t c_countOfImagesToGrab = 10000;
bool setSequenceGrouping(INodeMap *nodemap, vector<double>& seqExposureTime, int totalNum = 3, bool setBinn=false, int Horizontal = 4, int Vertical = 4)
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
			CCommandPtr SetStore(nodemap->GetNode("SequenceSetStore"));
			if (sequenceSetIndex && IsWritable(sequenceSetIndex))
			{
				for (int i = 0; i < totalNum; i++)
				{
					if (setBinn)
					{
						sequenceSetIndex->SetValue(i);
						assert(sequenceSetIndex->GetValue() == i);
						if (sequenceSetExposureTime && IsWritable(sequenceSetExposureTime))
						{
							sequenceSetExposureTime->SetValue(seqExposureTime[i]);
							assert(sequenceSetExposureTime->GetValue() == seqExposureTime[i]);

						}
						setBinning(*nodemap, Horizontal, Vertical);
						SetStore->Execute();
					}
					else
					{
						sequenceSetIndex->SetValue(i);
						assert(sequenceSetIndex->GetValue() == i);
						if (sequenceSetExposureTime && IsWritable(sequenceSetExposureTime))
						{
							sequenceSetExposureTime->SetValue(seqExposureTime[i]);
							assert(sequenceSetExposureTime->GetValue() == seqExposureTime[i]);
						}
						SetStore->Execute();
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