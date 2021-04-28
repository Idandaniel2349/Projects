#include "os.h"

//extract the 9 vpn bits starting in bit index start
uint64_t ExtractVPNSlice(uint64_t vpn, uint64_t start){
    // make an int with first 9 bits as 1, move the vpn slice we need to the first 9 bits and
    // bitwise and the two to get the result we want.
    return (uint64_t)((1<<9)-1)&(vpn>>start);
}

//A function to create/destroy virtual memory mappings in a page table
void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn){
    uint64_t vpnSliced[5];
    uint64_t currentPTE;
    uint64_t * currentPT;

    for(int i=0;i<5;i++){//slice the vpn
        vpnSliced[i]=ExtractVPNSlice(vpn, (i*9));
    }

    if(ppn==NO_MAPPING){
        //destroy vpn mapping
        for(int i=4;i>0;i--){//slice the vpn
            currentPT = (uint64_t *) phys_to_virt(pt<<12);
            currentPTE = currentPT[vpnSliced[i]];
            if((currentPTE&1)==0){
                currentPT[vpnSliced[i]] = (alloc_page_frame()<<12);// define the PTE leading to the next level with bit valid off
                currentPTE = currentPT[vpnSliced[i]];
            }
            else {
                currentPT[vpnSliced[i]] = (currentPTE >> 1) << 1;//turn the valid bit off
            }
            pt=currentPTE>>12;//get the new page table
        }
        currentPT = (uint64_t *) phys_to_virt(pt<<12);
        currentPT[vpnSliced[0]]=NO_MAPPING;//make the valid bit 0

    }
    else{
        //create vpn mapping
        for(int i=4;i>0;i--){//slice the vpn
            currentPT = (uint64_t *) phys_to_virt(pt<<12);
            currentPT[vpnSliced[i]] = (alloc_page_frame()<<12) |1;// define the PTE leading to the next level with bit valid on
            currentPTE = currentPT[vpnSliced[i]];
            pt=currentPTE>>12;//get the new page table
        }
        currentPT = (uint64_t *) phys_to_virt(pt<<12);
        currentPT[vpnSliced[0]]=(ppn<<12)|1;//set the PTE in the last level with the ppn we need
    }

}

//A function to query the mapping of a virtual page number in a page table
uint64_t page_table_query(uint64_t pt, uint64_t vpn){
    uint64_t vpnSliced[5];
    uint64_t currentPTE;
    uint64_t * currentPT;



    for(int i=0;i<5;i++){//slice the vpn
        vpnSliced[i]=ExtractVPNSlice(vpn, i*9);
    }

    for(int i=4;i>0;i--){//walk on the trie
        currentPT = (uint64_t *) phys_to_virt(pt<<12);
        currentPTE=currentPT[vpnSliced[i]];

        if((currentPTE&1)==0){//checking if bit valid is on
            return NO_MAPPING;
        }
        pt=currentPTE>>12;//get the new page table
    }
    currentPT = (uint64_t *)phys_to_virt(pt<<12);
    currentPTE=currentPT[vpnSliced[0]];
    if((currentPTE&1)==0){//checking if bit valid is on
        return NO_MAPPING;
    }

    printf("\033%d\n",currentPTE>>12);
    return  (currentPTE>>12);//return the physical page number

}
