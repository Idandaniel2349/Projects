using UnityEngine;

public class ChestOpen : Interactable
{

    public Item helemt;
    public Item weapon;
    public override void Interact()
    {
        base.Interact();

        AudioManager.instance.Play("Use");

        if (EquipmentMangaer.instance.openedchest == false)//if we havem't opened the chest yet
        {
            getItems();
            EquipmentMangaer.instance.openedchest = true;
        }
    }
    public void getItems()//get items from chest
    {
        Inventory.instance.Add(helemt);
        Inventory.instance.Add(weapon);
    }
    
}
