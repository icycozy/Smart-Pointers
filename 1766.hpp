int *getNewBlock(int n);
void freeBlock(const int *block, int n);

struct node{
    bool use; // 是否为空
    int s,pos; // s记录大小，pos记录在内存条中的位置
};
struct block{
    int *start=nullptr,size,c; // c记录链表结点个数
    int nxt[1010],head;
    node list[1010];  // 用链表存储数据块
}blo[1010];
int cnt;

class Allocator
{
private:

public:
    Allocator(){}

    ~Allocator()
    {
        for(int i=1;i<=cnt;i++){
            if(blo[i].start!=nullptr)
                freeBlock(blo[i].start,blo[i].size/1024);
            blo[i].start=nullptr;
        }
    }

    int *allocate(int n)
    {
        for(int i=1;i<=cnt;i++){
            for(int j=blo[i].head;j;j=blo[i].nxt[j]){
                if(!blo[i].list[j].use && blo[i].list[j].s>=n){
                    if(blo[i].list[j].s==n){
                        blo[i].list[j].use=1;
                    }
                    else{
                        blo[i].c++;
                        blo[i].list[blo[i].c].s=blo[i].list[j].s-n;
                        blo[i].list[j].s=n;
                        blo[i].list[blo[i].c].pos=blo[i].list[j].pos+n;
                        blo[i].list[j].use=1;
                        blo[i].nxt[blo[i].c]=blo[i].nxt[j];
                        blo[i].nxt[j]=blo[i].c;
                    } // 把原来的内存块裂成两块
                    return blo[i].start+blo[i].list[j].pos;
                }
            }
        }
        ++cnt;
        int nn=((n-1)/1024)+1;
        blo[cnt].start=getNewBlock(nn);
        blo[cnt].size=nn*1024;
        blo[cnt].c=1;
        blo[cnt].head=1;
        blo[cnt].list[1].use=1;
        blo[cnt].list[1].s=n;  // 申请新的内存条
        if(blo[cnt].size>n){
            blo[cnt].c++;
            blo[cnt].nxt[1]=2;
            blo[cnt].list[2].pos=n;
            blo[cnt].list[2].s=blo[cnt].size-n;
        } // 第一块存数据，第二块空
        return blo[cnt].start;
    }

    void deallocate(int *pointer, int n)
    {
        int p,pp;
        for(int i=1;i<=cnt;i++){
            if(blo[i].start<=pointer&&pointer<blo[i].start+blo[i].size){
                p=i;
                for(int j=blo[i].head;j;j=blo[i].nxt[j]){
                    if(blo[i].list[j].pos+blo[i].start==pointer){
                        pp=j;
                        break;
                    }
                }
                break;
            }
        }
        if(pp==blo[p].head){
            int j=blo[p].nxt[pp]; 
            if(blo[p].list[j].use || !j){
                blo[p].list[pp].use=0;
            }
            else{
                int x=++blo[p].c;
                blo[p].head=x;
                blo[p].list[x].s=blo[p].list[pp].s+blo[p].list[j].s;
                blo[p].nxt[x]=blo[p].nxt[j];
            } // 与后一块（空块）合并
            p=pp=0;
            return;
        }
        int pre=0;
        for(int j=blo[p].head;j;j=blo[p].nxt[j]){
            if(blo[p].nxt[j]==pp){
                int k=blo[p].nxt[pp]; // pre->j->pp->k
                if(blo[p].list[j].use){
                    if(blo[p].list[k].use||!k)
                        blo[p].list[pp].use=0;
                    else{
                        int x=++blo[p].c;
                        blo[p].list[x].pos=blo[p].list[pp].pos;
                        blo[p].list[x].s=blo[p].list[pp].s+blo[p].list[k].s;
                        blo[p].nxt[x]=blo[p].nxt[k];
                        blo[p].nxt[j]=x;
                    }
                }
                else{
                    int x=++blo[p].c;
                    if(blo[p].list[k].use||!k){
                        blo[p].list[x].pos=blo[p].list[j].pos;
                        blo[p].list[x].s=blo[p].list[j].s+blo[p].list[pp].s;
                        blo[p].nxt[x]=k;
                        if(!pre)
                            blo[p].head=x;
                        else blo[p].nxt[pre]=x;
                    }
                    else{
                        blo[p].list[x].pos=blo[p].list[j].pos;
                        blo[p].list[x].s=blo[p].list[j].s+blo[p].list[pp].s+blo[p].list[k].s;
                        blo[p].nxt[x]=blo[p].nxt[k];
                        if(!pre)
                            blo[p].head=x;
                        else blo[p].nxt[pre]=x;
                    } // 三块合并
                }
                p=pp=0;
                return;
            }
            pre=j;
        }
    }
};