using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName = "New Equipment",menuName= "Inventory/Equipment")]
public class Equipment : Item
{
    public EquipmentSlot equipSlot;
    public int armourModifier;
    public int damageModifier;

    public override void Use()
    {
        base.Use();
        EquipmentMangaer.instance.Equip(this);//equip the item
        RemoveFromInventory();//remove from inventory
    }


}

public enum EquipmentSlot {Head, Weapon}
