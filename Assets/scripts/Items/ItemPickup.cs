using UnityEngine;

public class ItemPickup : Interactable
{

    public Item item;
    public override void Interact()//intreact with item
    {
        base.Interact();

        AudioManager.instance.Play("Use");
        Pickup();
    }

    void Pickup () //pick up the item and add to inventory(if there is sapce)
    {
        if (Inventory.instance.Add(item))
            Destroy(gameObject);

    }

}
