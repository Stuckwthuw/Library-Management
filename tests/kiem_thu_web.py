import json, pathlib, shutil, socket, subprocess, sys, tempfile, time, urllib.request, urllib.error, urllib.parse
exe=pathlib.Path(sys.argv[1]).resolve()
repo=pathlib.Path(__file__).resolve().parents[1]
run=pathlib.Path(tempfile.mkdtemp(prefix='library-web-'))
shutil.copytree(repo/'Web',run/'Web')
with socket.socket() as sock:
 sock.bind(('127.0.0.1',0)); port=sock.getsockname()[1]
base=f'http://127.0.0.1:{port}'
proc=None
version=1; token=''
def start():
 global proc
 proc=subprocess.Popen([str(exe),str(port)],cwd=run,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
 for _ in range(100):
  try: return call('status')
  except OSError: time.sleep(.05)
 raise AssertionError('server did not start')
def call(path,params=None,post=False,expected=200):
 global version,token
 data=urllib.parse.urlencode(dict(params or {},**({'version':version,'token':token} if post else {}))).encode()
 req=urllib.request.Request(base+'/api/v1/'+path+('' if post else '?'+data.decode()),data=data if post else None,headers={'Content-Type':'application/x-www-form-urlencoded'} if post else {})
 try:
  with urllib.request.urlopen(req,timeout=8) as r: code=r.status; result=json.load(r)
 except urllib.error.HTTPError as e: code=e.code; result=json.load(e)
 assert code==expected,(path,code,result)
 version=result.get('version',version)
 if path=='status': token=result['data']['token']
 return result.get('data')
try:
 start()
 assert urllib.request.urlopen(base).status==200
 call('readers/add',dict(ho='Le Van',ten='Hoang Hieu',phai='Nam',status=1),True,400)
 call('readers/add',dict(ho='  le   van ',ten='hiếu',phai='Nam',status=1),True)
 readers=call('readers')['items']; assert readers[0]['ten']=='Hiếu'; ident=readers[0]['maThe']
 call('books/add',dict(isbn=' a ',title='Sách thử',author='Nguyen Van',genre='Tin hoc',pages=120,year=2026),True)
 call('copies/add',dict(isbn='A',count=2,shelf='Kệ A1'),True)
 assert call('copies',dict(isbn='A'))['total']==2
 preview=call('preview',dict(id=ident,code='A_1')); assert preview['viTri']=='Kệ A1'
 call('borrow',dict(id=ident,code='A_1'),True)
 call('borrow',dict(id=ident,code='A_2'),True,409)
 call('readers/delete',dict(id=ident),True,409)
 assert call('loans',dict(id=ident))['total']==1
 call('return',dict(id=ident,code='A_1'),True)
 call('borrow',dict(id=ident,code='A_2'),True)
 call('lost',dict(id=ident,code='A_2'),True)
 call('readers/delete',dict(id=ident),True)
 assert call('top')['items'][0]['luotMuon']==2
 call('readers',dict(size='101'),expected=400)
 call('readers',dict(page='1.5'),expected=400)
 # Mô phỏng lưu thất bại trước công bố journal, không phát lại thao tác thêm.
 (run/'Data'/'DocGia.txt.prepare').mkdir()
 call('readers/add',dict(ho='Le Van',ten='An',phai='Nam',status=1),True,503)
 assert call('status')['dirty']
 call('readers',expected=503)
 (run/'Data'/'DocGia.txt.prepare').rmdir()
 call('save',{},True)
 assert call('readers')['total']==1
 stale=version-1; current=version; version=stale
 call('readers/delete',dict(id=ident),True,409); version=current
 # Chặn host ngoài localhost và đường dẫn traversal.
 req=urllib.request.Request(base+'/api/v1/status',headers={'Host':'example.com'})
 try: urllib.request.urlopen(req); raise AssertionError('Host accepted')
 except urllib.error.HTTPError as e: assert e.code==403
 call('stop',{},True); proc.wait(timeout=5)
 start(); assert call('top')['items'][0]['luotMuon']==2
 call('stop',{},True); proc.wait(timeout=5)
 # D? li?u c? ph?i ???c x?c nh?n tr?n web, d?ng tr??c x?c nh?n gi? nguy?n file.
 old="DOCGIA_V3 | 1 | 50000 | Le Van | Hoang Hieu | Nam | 1 | 0 | 0\n"
 (run/'Data'/'DocGia.txt').write_text(old,encoding='utf-8')
 (run/'Data'/'DauSach.txt').write_text("DAUSACH_V1 | 0\n",encoding='utf-8')
 assert start()['migration']
 call('readers',expected=503)
 call('stop',{},True); proc.wait(timeout=5)
 assert (run/'Data'/'DocGia.txt').read_text(encoding='utf-8')==old
 start(); call('migrate',{},True)
 assert call('readers')['items'][0]['ten']=='Hieu'
 # HTTP malformed, token invalid, path traversal, unsupported transfer encoding.
 original_token=token; token='invalid'; call('save',{},True,403); token=original_token
 for target,headers,expected in [('/../Data/DocGia.txt','',404),('/api/v1/status','Transfer-Encoding: chunked\r\n',400),('/api/v1/status','Content-Length: 999999\r\n',400)]:
  with socket.create_connection(('127.0.0.1',port)) as sock:
   sock.sendall((f'GET {target} HTTP/1.1\r\nHost: 127.0.0.1:{port}\r\n'+headers+'\r\n').encode())
   assert f' {expected} '.encode() in sock.recv(1024)
 call('stop',{},True); proc.wait(timeout=5)
 print('PASS: web API, Unicode, persistence, preview, duplicate ISBN, save failure, stale version, Host protection. '+str(run))
finally:
 if proc and proc.poll() is None: proc.terminate(); proc.wait(timeout=5)
