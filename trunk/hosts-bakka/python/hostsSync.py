#-* encoding: utf8 *-

def read(url):                                             
    import urllib2
	#http://wiki.daumcorp.com/potz/TvpotClientHosts?action=raw
    con = urllib2.urlopen(url)
    return con.read()

def readfile(path):
    f=open(path,'r')
    content = f.read() 
    f.close()
    return content

def writefile(path,content):
    f=open(path, 'w')
    f.write(content)
    f.close()
    return True

def backupfile(path,backuppath):
    import shutil
    shutil.copyfile(path, backuppath)
    return nil

def get_hostsfile():
    import os
    hostsfile={'posix':"/etc/hosts",'nt':'c:\\windows\\system32\\drivers\\etc\hosts'}[os.name]
    return hostsfile


import re
#########################################
# wiki의 내용가져와서 {{{ }}} 브라켓,전후 제거 #
#########################################
webcont = read("http://wiki.daumcorp.com/potz/TvpotClientHosts?action=raw")
webcont = re.sub(r'.*{{{','',webcont)
webcont = re.sub(r'}}}.*','',webcont)
webcont = webcont.strip()
print "[##AUTOSYNC## list]"
print webcont
print ""

############################
# os에 따른 host파일 위치 가져옴 #
############################
hostsfile = get_hostsfile()

##################################################
# 로컬의 hosts 파일 읽어서 ##AUTOSYNC## 가 적힌것은 제외 #
##################################################
print "[hostfile path]"
print hostsfile
print ""
hosts = readfile(hostsfile)
p = re.compile(r'^.*?##AUTOSYNC##\s*$',re.MULTILINE)
hosts = p.sub("",hosts)
hosts = hosts.strip()

#############################################################
# 위에서 처리한 로컬의 호스트파일내용과 웹에서 가져온 호스트파일내용을 합침 #
#############################################################
writefile(hostsfile,hosts+"\n\n##AUTOSYNC##\n"+webcont)


