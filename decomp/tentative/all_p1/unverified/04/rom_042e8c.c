/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x042e8c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_42e8c(void)

{
  byte bVar1;
  int extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  candidate_4256();
  *unaff_A6 = 0x42ea2;
  uVar2 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar2) {
    *(undefined2 *)(unaff_A6 + 0x22) = *(undefined2 *)((int)unaff_A6 + 0x86);
    bVar3 = false;
    candidate_40bc();
    if (bVar3) {
      *(undefined4 *)(extraout_A0 + 0x6e) = *(undefined4 *)((int)unaff_A6 + 0x6e);
      *(undefined4 *)(extraout_A0 + 0x72) = *(undefined4 *)((int)unaff_A6 + 0x72);
      *(undefined1 *)(extraout_A0 + 0x80) = *(undefined1 *)(unaff_A6 + 0x20);
    }
    candidate_1acf2();
    *unaff_A6 = candidate_42e74;
  }
  bVar1 = *(byte *)(*(int *)((int)unaff_A6 + 0x72) + 0x6e +
                   (int)(short)(ushort)*(byte *)((int)unaff_A6 + 0x83));
  if (bVar1 != *(byte *)(unaff_A6 + 0x21)) {
    *(byte *)(unaff_A6 + 0x21) = bVar1;
    *unaff_A6 = *(undefined4 *)
                 (*(int *)((short)((ushort)*(byte *)((int)unaff_A6 + 0x83) << 2) + 0x453a6) +
                 (int)(short)((bVar1 & 0x7f) * 4));
  }
  *(undefined1 *)(unaff_A6 + 0x20) = *(undefined1 *)(*(int *)((int)unaff_A6 + 0x6e) + 0x80);
  return;
}
