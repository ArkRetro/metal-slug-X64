/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09b550.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9b550(void)

{
  undefined4 *unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x24) & 0x40) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
    *(undefined1 *)((int)unaff_A6 + 0x5a) = *(undefined1 *)((int)unaff_A6 + 0x86);
    if ((*(byte *)(unaff_A6 + 0x24) & 0x10) == 0) {
      candidate_406b6();
    }
    else {
      candidate_407b6();
    }
  }
  else {
    candidate_9d5ba();
    candidate_9ce50();
    *unaff_A6 = 0x9b59a;
  }
                    /* WARNING: Could not recover jumptable at 0x0009b594. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)*unaff_A6)();
  return;
}
