/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cbc32.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cbc32(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_4256();
  *(undefined2 *)(unaff_A6 + 0x1c) = 3;
  *unaff_A6 = 0xcbc4e;
  uVar2 = 0;
  candidate_cb878();
  candidate_427a();
  if ((bool)uVar2) {
    sVar1 = *(short *)(unaff_A6 + 0x1c) + -1;
    *(short *)(unaff_A6 + 0x1c) = sVar1;
    if (sVar1 == 0) {
      *unaff_A6 = candidate_cbc82;
    }
    else {
      candidate_4256();
    }
  }
  if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_cbd5c;
  }
  return;
}
