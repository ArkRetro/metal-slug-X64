/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x027576.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_27576(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_27820();
  *unaff_A6 = 0x27588;
  candidate_1abc8();
  candidate_1ac38();
  *(byte *)(unaff_A6 + 0x21) = (byte)(*(char *)((int)unaff_A6 + 0x85) + 4U) >> 3;
  candidate_427a();
  *(char *)(unaff_A6 + 0x23) = *(char *)(unaff_A6 + 0x23) + '\x01';
  if ('\x02' < *(char *)(unaff_A6 + 0x23)) {
    *(undefined1 *)(unaff_A6 + 0x23) = 0;
  }
  *(undefined1 *)(unaff_A6 + 5) =
       *(undefined1 *)((int)unaff_A6 + (short)(*(byte *)(unaff_A6 + 0x23) & 3) + 0x19);
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_27616;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_27616;
  }
  uVar2 = false;
  candidate_19424();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_27616;
    uVar2 = 0;
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
