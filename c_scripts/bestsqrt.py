import itertools
import subprocess
import json
import threading


class ReturnFunctionTree:
    
    @classmethod
    def generate_all(cls,case):
        if case == 1:
            yield [1]
            return
        if case == 2:
            yield [[1],1]
            return
        if case == 3:
            yield [[1],[1],1]
            yield [[[1],1],1]
            return
        case -= 1
        for first_split in range(case,0,-1):
            split = [1]*first_split
            split[0] += case-first_split
            i = 0
            if split[0] == 1: i = len(split)-1
            while True:
                for tree in itertools.product(*[list(cls.generate_all(x)) for x in split]):
                    yield list(tree)+[1]
                if i+1 == len(split):
                    carry = split[i]+1
                    split[i] = 1
                    for i in range(i-1,-1,-1):
                        if split[i] >= 2:
                            break
                    else:
                        break
                else:
                    carry = 2
                split[i] -= 1
                split[i+1] = carry
                i += 1


    @classmethod
    def get_cost(cls,tree,result=None,basecost=0.0):
        if result == None:
            result = []
        for x in tree:
            if isinstance(x,list):
                cls.get_cost(x,result,basecost+2.5)
            else:
                result.append(basecost)
            basecost += 1.0
        return result


    @classmethod
    def print(cls,tree,indent=''):
        result = []
        for x in tree:
            if isinstance(x,list):
                result.append(indent+'execute if ... run return run function ...\n')
                result.append(cls.print(x,'    '+indent))
            else:
                result.append(indent+'command')
        return ''.join(result)


    @classmethod
    def find_best(cls,cost):
        case = len(cost)
        cost_sorted = sorted(cost,reverse=True)
        min_trees = []
        min_maxcost = 2147483647
        min_sumcost = 2147483647
        for tree in cls.generate_all(case):
            tree_cost = cls.get_cost(tree)
            tree_cost_sorted = sorted(tree_cost)
            maxcost = 0
            sumcost = 0
            for c1,c2 in zip(cost_sorted,tree_cost_sorted):
                maxcost = max(maxcost,c1+c2)
                sumcost += c1+c2
            if maxcost == min_maxcost:
                if sumcost == min_sumcost:
                    min_trees.append(tree)
                elif sumcost < min_sumcost:
                    min_sumcost = sumcost
                    min_trees.clear()
                    min_trees.append(tree)
            elif maxcost < min_maxcost:
                min_maxcost = maxcost
                min_sumcost = sumcost
                min_trees.clear()
                min_trees.append(tree)
        return min_trees


    @classmethod
    def print_with_cost(cls,tree,cost,indent='',first=True):
        if first:
            cls.get_cost(tree)
            cost = sorted(list(enumerate(cost)),key=lambda x:x[1],reverse=True)
        for x in tree:
            if isinstance(x,list):
                result.append(indent+'execute if ... run return run function ...\n')
                result.append(cls.print_with_cost(x,'    '+indent))
            else:
                result.append(indent+'command')
        return ''.join(result)




def fileappend(filename,text):
    with open(filename,'a') as f:
        f.write(text)
def fileappendsafe(filename,text):
    if fileappendsafe.threadbefore != None:
        fileappendsafe.threadbefore.join()
    fileappendsafe.threadbefore = threading.Thread(target=fileappend,args=[filename,text])
    fileappendsafe.threadbefore.start()
fileappendsafe.threadbefore = None



def get_estimate(loops,string,startx):
    print(loops,string,startx)
    if get_estimate.cache == None:
        get_estimate.cache = dict()
        try:
            with open('isqrt_estimate_cache.txt','r') as f:
                lines = f.read().rstrip().split('\n')
            for line in lines:
                if not line: continue
                data = json.loads(line)
                key = ' '.join(['isqrt_l%d'%data['HERONS_LOOP'],data['type'],str(data['startx'])])
                get_estimate.cache[key] = data
        except:
            pass
    if startx >= 93605625:
        if startx >= 93605625 and string == 'linear_estimate' and loops == 2:
            return {'HERONS_LOOP':2,'type':'linear_estimate','startx':startx,'endx':2147483647,'div':50704,'cmin':9628,'cmax':9628}
        if startx >= 262148481 and string == 'rational_estimate' and loops == 1:
            return {'HERONS_LOOP':1,'type':'rational_estimate','startx':startx,'is_div':True,'endx':2147483647,'a':95355,'b':26467,'cmin':89876,'cmax':89876}
        if startx >= 1090518529 and string == 'linear_estimate' and loops == 1:
            return {'HERONS_LOOP':1,'type':'linear_estimate','startx':startx,'endx':2147483647,'div':79088,'cmin':19493,'cmax':19493}
        if startx >= 93605625 and string == 'rational_estimate' and loops == 0:
            return {'HERONS_LOOP':0,'type':'rational_estimate','startx':startx,'is_div':True,'endx':2147483647,'a':41067,'b':17584,'cmin':77074,'cmax':77074}
    try:
        cmd = ['isqrt_l%d'%loops,string,str(startx)]
        key = ' '.join(cmd)
        if key in get_estimate.cache:
            return get_estimate.cache[key]
        with subprocess.Popen(cmd, stdout=subprocess.PIPE, text=True) as proc:
            proc.wait()
            if proc.returncode == 0:
                data = proc.stdout.readlines()[-1]
                fileappendsafe('isqrt_estimate_cache.txt',data.rstrip()+'\n')
                data = json.loads(data)
                get_estimate.cache[key] = data
                print(data)
                return data
            else:
                print(cmd,'returned',proc.returncode)
                input()
                exit()
    except Exception as e:
        print(e)
        input()
        exit()
get_estimate.cache = None




available_estimates = [{'HERONS_LOOP':0,'type':'rational_estimate'},{'HERONS_LOOP':1,'type':'linear_estimate'},{'HERONS_LOOP':1,'type':'rational_estimate'},{'HERONS_LOOP':2,'type':'linear_estimate'}]


class EstimateTree:
    def __init__(self,root,data,level):
        self.root = root
        self.data = data
        self.level = level
        self.children = None

    def search_next_depth(self,result):
        if self.children == None:
            self.children = []
            startx = self.data['endx']+1
            for l in range(self.level,len(available_estimates)):
                estimate = available_estimates[l]
                data = get_estimate(estimate['HERONS_LOOP'],estimate['type'],startx)
                child = EstimateTree(self,data,l)
                self.children.append(child)
                if data['endx'] == 2147483647:
                    root = self.root
                    arr = [child.data,self.data]
                    while root != None:
                        arr.append(root.data)
                        root = root.root
                    result.append(arr)
        else:
            for i in range(len(self.children)-1,-1,-1):
                r = self.children[i].search_next_depth(result)
                if r == False:
                    del self.children[i]
        return len(self.children) >= 1




maintree = EstimateTree(None,{'endx':-1},0)
result = []
maintree.search_next_depth(result)
maintree.search_next_depth(result)
maintree.search_next_depth(result)
maintree.search_next_depth(result)
print(result)



# cost = [1,1,1,1,1,1,1,1,1,1,1,1]
# trees = ReturnFunctionTree.find_best(cost)
# print(trees)

# print(get_estimate(1,'rational_estimate',0))
