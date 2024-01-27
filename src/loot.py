
def wr(xmin,xmax,has_condition,fwrite):
    fwrite('{')
    if has_condition:
        fwrite('"conditions":[{"condition":"value_check","range":{"min":%d},"value":{"type":"score","score":"sqrt","target":{"type":"fixed","name":"#x"}}}],'%(xmin*xmin))
    if xmin == xmax:
        fwrite('"type":"item","name":"cod","functions":[{"function":"set_nbt","tag":"{r:%d}"}]}'%xmin)
    elif xmax-xmin == 2:
        fwrite('"type":"alternatives","children":[')
        wr(xmax,xmax,True,fwrite)
        fwrite(',')
        wr(xmin+1,xmin+1,True,fwrite)
        fwrite(',')
        wr(xmin,xmin,False,fwrite)
        fwrite(']}')
    else:
        xmid = (xmin+xmax+1)//2
        fwrite('"type":"alternatives","children":[')
        wr(xmid,xmax,True,fwrite)
        fwrite(',')
        wr(xmin,xmid-1,False,fwrite)
        fwrite(']}')


with open('loot.json','w') as f:
    fwrite = f.write
    fwrite('{"type":"command","pools":[{"rolls":1,"entries":[')
    wr(-1,46340,False,fwrite)
    fwrite(']}]}')
