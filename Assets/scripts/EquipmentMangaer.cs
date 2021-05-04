using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EquipmentMangaer : MonoBehaviour
{
    public static EquipmentMangaer instance;
    public bool openedchest;//keep track on whatever we opened the chest

    Equipment [] currentEquipment;
    public GameObject Helmet;
    public GameObject Weapon;

    public delegate void OnEquipmentChanged(Equipment newItem, Equipment oldItem);
    public OnEquipmentChanged onEquipmentChanged;

    Inventory inventory;

    void Awake()
    {
        instance = this;
        this.openedchest = false;
    }

    void Start()
    {
        inventory = Inventory.instance;
        int numSlots = System.Enum.GetNames(typeof(EquipmentSlot)).Length;
        currentEquipment = new Equipment[numSlots];
    }

    public void Equip(Equipment newItem)
    {
        int slotIndex = (int)newItem.equipSlot;
        Equipment oldItem = currentEquipment[slotIndex];

        if (currentEquipment[slotIndex] != null)
        {
            inventory.Add(oldItem);
        }

        if (onEquipmentChanged != null)
        {
            onEquipmentChanged.Invoke(newItem, oldItem);
        }

        currentEquipment[slotIndex] = newItem;
        if (slotIndex == 0)//equip the helmet on the player
        {
            Helmet.SetActive(true);
        }

        if (slotIndex == 1)//equip the weapon on the player
        {
            Weapon.SetActive(true);
        }
    }

    public void unEquip(int slotIndex)
    {
        if (currentEquipment[slotIndex] != null)
        {
            inventory.Add(currentEquipment[slotIndex]);
            if (onEquipmentChanged != null)
            {
                onEquipmentChanged.Invoke(null, currentEquipment[slotIndex]);
            }
            currentEquipment[slotIndex] = null;
            if (slotIndex == 0)//equip the helmet on the player
            {
                Helmet.SetActive(false);
            }

            if (slotIndex == 1)//equip the weapon on the player
            {
                Weapon.SetActive(false);
            }

        }
    }

    public void unequipAll()
    {
        for(int i = 0; i < currentEquipment.Length; i++)
        {
            unEquip(i);
        }
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.U))
        {
            unequipAll();
        }
    }
}
