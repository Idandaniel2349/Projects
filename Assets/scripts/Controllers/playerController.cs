using UnityEngine.EventSystems;
using UnityEngine;

[RequireComponent(typeof(playerMotor))]
public class playerController : MonoBehaviour
{
    public Interactable focus;

    public LayerMask movementMask;
    Camera cam;
    playerMotor motor;

    // Start is called before the first frame update
    void Start()
    {
        cam = Camera.main;
        motor= GetComponent<playerMotor>();
    }

    // Update is called once per frame
    void Update()
    {
        if (EventSystem.current.IsPointerOverGameObject())
        {
            return;
        }

        if (Input.GetMouseButtonDown(0))//if pressing the left mouse button then move to directed position
        {
            Ray ray = cam.ScreenPointToRay(Input.mousePosition);
            RaycastHit hit;

            if(Physics.Raycast(ray, out hit,100,movementMask))
            {
                //move our player to what we hit
                motor.MoveToPoint(hit.point);

                RemoveFocus();

            }
        }

        if (Input.GetMouseButtonDown(1))//if pressing the right mouse button intearct with an object
        {
            Ray ray = cam.ScreenPointToRay(Input.mousePosition);
            RaycastHit hit;

            if (Physics.Raycast(ray, out hit, 100))
            {
                //cheack if we hit an intractable and if we did set it as our focus
                Interactable interactable = hit.collider.GetComponent<Interactable>();
                if (interactable != null)
                {
                    SetFocus(interactable);
                }

            }
        }

    }

    void SetFocus(Interactable newFocus)
    {
        if (newFocus != focus)//if there is already a focus
        {
            if (focus != null)//if previous focus in not null then defocus from it
                focus.OnDeFocused();
            focus = newFocus;//set new focus
            motor.FollowTarget(newFocus);//follow new focus
        }
        newFocus.OnFocused(transform);//focus on new focus
    }

    void RemoveFocus()
    {
        if(focus!=null)
            focus.OnDeFocused();

        focus = null;
        motor.StopFollowTarget();
    }
}
