/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a45ec.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a45ec(void)

{
  int iVar1;
  undefined4 *unaff_A6;

  candidate_a49cc();
  candidate_1ac38();
  if ((*(byte *)(unaff_A6 + 0x18) & 1) == 0) {
    if (unaff_A6[0x21] == 0) {
      candidate_427a();
      candidate_a48c2();
    }
    else {
      iVar1 = candidate_a49b0();
      iVar1 = unaff_A6[0x21] - iVar1;
      unaff_A6[0x21] = iVar1;
      if (iVar1 < 0) {
        unaff_A6[0x21] = 0;
      }
    }
  }
  else {
    candidate_427a();
    if ((*(byte *)(unaff_A6 + 0x18) & 2) == 0) {
      if (unaff_A6[0x22] == 0) {
        candidate_a492e();
      }
      else {
        iVar1 = candidate_a49b0();
        iVar1 = unaff_A6[0x22] - iVar1;
        unaff_A6[0x22] = iVar1;
        if (iVar1 < 0) {
          unaff_A6[0x22] = 0;
        }
      }
    }
    else {
      *unaff_A6 = candidate_41ac;
    }
  }
  if ((*(short *)((int)unaff_A6 + 0x6e) == 2) &&
     (*(short *)(unaff_A6 + 0x1e) <= *(short *)(unaff_A6 + 0xb))) {
    unaff_A6[0xc] = 0;
    unaff_A6[0xd] = 0;
    unaff_A6[0x25] = 0;
    unaff_A6[0x26] = 0;
    *(undefined2 *)((int)unaff_A6 + 0x6e) = 0;
  }
  candidate_a472c();
  return;
}
