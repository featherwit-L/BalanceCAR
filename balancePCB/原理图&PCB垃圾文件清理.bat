del *.SchDocPreview /s
del *.PcbDocPreview /s 
del *.PrjPCBStructure /s 
del *.drc /s
del *.LOG /s 
del *.htm /s 
::del *.OutJob /s 
del Documents/*.SchDocPreview/s
del Documents/*.PcbDocPreview/s
for /r /d %%b in (__Previews) do rd "%%b" /s /q 
::ɾ����ǰĿ¼�µ�����__Previews�ļ���
for /r /d %%b in (History) do rd "%%b" /s /q 
::ɾ����ǰĿ¼�µ�����History�ļ���
for /r /d %%b in (Project?Logs?for*) do rd "%%b" /s /q 
::ɾ����ǰĿ¼�µ����д��ַ���Project Logs for���ļ���
for /r /d %%b in (Project?Outputs?for*) do rd "%%b" /s /q   
::ɾ����ǰĿ¼�µ����д��ַ���Project Outputs for���ļ���
exit
::˵��:rdָ������ɾ���ļ���
::delָ������ɾ���ļ�
::�����б༭�����������ɾ����