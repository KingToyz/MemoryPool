#pragma once
#include <new>
#include <iostream>
template<class T>
class MemoryPool {
    using value_type = T;
    using value_pointer = T*;
    using value_reference = T&;
    using value_const = const T;
    public:
        MemoryPool(int AllocateNum = 1):MaxAllocateNum(AllocateNum) {
            FreeHeadSlot = new slot;
            FreeHeadSlot -> next = nullptr;
            UsedHeadSlot = new slot;
            UsedHeadSlot -> next = nullptr;
            allocateSlots();
        }
        ~MemoryPool() {
            slot_pointer curr = UsedHeadSlot->next;
            while(curr != nullptr) {
                slot_pointer _next = curr->next;
                destory(curr);
                delete curr;
                curr = _next;
            }
            curr = FreeHeadSlot->next;
            while(curr != nullptr) {
                slot_pointer _next = curr->next;
                delete curr;
                curr = _next;
            }
            if(UsedHeadSlot != nullptr) {
                delete UsedHeadSlot;
            }
            if(FreeHeadSlot != nullptr) {
                delete FreeHeadSlot;
            }
        }
        template<class... Args>
        value_pointer Allocate(Args&&... args) {
            slot_pointer curr = FreeHeadSlot->next;
            if (curr != nullptr) {
                if(curr->next == nullptr) {
                    allocateSlots();
                }
                FreeHeadSlot->next = curr->next;
                std::cout << "Allocate:" << curr << std::endl;
                std::cout << "Allocate:" << &(curr->next) << std::endl;
                std::cout << "Allocate:" << &(curr->prev) << std::endl;
                std::cout << "Allocate:" << &(curr->data) << std::endl;
                curr->next = UsedHeadSlot->next;
                UsedHeadSlot->next = curr;
                curr->prev = UsedHeadSlot;
                if(curr->next != nullptr) {
                    curr->next->prev = curr;
                }
                return new (curr->data) value_type (std::forward<Args>(args)...);
            } else {
                allocateSlots();
                curr = FreeHeadSlot->next;
                FreeHeadSlot->next = curr->next;
                curr->next = UsedHeadSlot->next;
                UsedHeadSlot->next = curr;
                curr->prev = UsedHeadSlot;
                if(curr->next != nullptr) {
                    curr->next->prev = curr;
                }
                std::cout << "Allocate:" << curr << std::endl;
                std::cout << "Allocate:" << &(curr->next) << std::endl;
                std::cout << "Allocate:" << &(curr->prev) << std::endl;
                std::cout << "Allocate:" << &(curr->data) << std::endl;
                return new (curr->data) value_type (std::forward<Args>(args)...);
            }
        }
        void Deallocate(value_pointer p) {
            p->~value_type();
            std::cout << "Deallocate curr:" << (void*)(reinterpret_cast<char*>(p) - sizeof(slot_pointer)*2) << std::endl;
            std::cout << "Deallocate curr->data:" << (void*)reinterpret_cast<char*>(p) << std::endl;
            slot_pointer sp = reinterpret_cast<slot_pointer>(reinterpret_cast<char*>(p) - sizeof(slot_pointer)*2);
            sp->prev->next = sp->next;
            if(sp->next != nullptr) {
                sp->next->prev = sp->prev;
            }
            sp->next = FreeHeadSlot->next;
            FreeHeadSlot->next = sp;
        }


        MemoryPool& operator=(const MemoryPool& memoryPool) = delete;
        MemoryPool& operator=(MemoryPool&& memoryPool) = delete;


        MemoryPool(const MemoryPool& pool) = delete;
        MemoryPool(MemoryPool&& pool) = delete;
    private:
        
        using data_type = char;
        struct slot
        {
            slot* next;
            slot* prev;
            data_type data[sizeof(value_type)];
        };

        int MaxAllocateNum;
        using slot_pointer = slot*;
        slot_pointer FreeHeadSlot;
        slot_pointer UsedHeadSlot;

        void allocateSlots() {
            for(int i = 0 ;i < MaxAllocateNum;i++) {
                slot_pointer sp = new slot;
                sp->next = FreeHeadSlot->next;
                FreeHeadSlot->next = sp;
            }
        }

        void destory(slot_pointer sp) {
            value_pointer vp = reinterpret_cast<value_pointer>(sp->data);
            vp->~value_type();
        }
};