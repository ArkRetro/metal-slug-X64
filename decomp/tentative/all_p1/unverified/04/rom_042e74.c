/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x042e74.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_42e74(void)

{
  short sVar1;
  byte bVar2;
  short sVar3;
  undefined4 *unaff_A6;

  candidate_1abc8();
  sVar1 = *(short *)(unaff_A6 + 0x22);
  sVar3 = sVar1 + -1;
  *(short *)(unaff_A6 + 0x22) = sVar3;
  if (sVar3 == 0 || sVar1 < 1) {
    *unaff_A6 = candidate_42e8c;
  }
  bVar2 = *(byte *)(*(int *)((int)unaff_A6 + 0x72) + 0x6e +
                   (int)(short)(ushort)*(byte *)((int)unaff_A6 + 0x83));
  if (bVar2 != *(byte *)(unaff_A6 + 0x21)) {
    *(byte *)(unaff_A6 + 0x21) = bVar2;
    *unaff_A6 = *(undefined4 *)
                 (*(int *)((short)((ushort)*(byte *)((int)unaff_A6 + 0x83) << 2) + 0x453a6) +
                 (int)(short)((bVar2 & 0x7f) * 4));
  }
  *(undefined1 *)(unaff_A6 + 0x20) = *(undefined1 *)(*(int *)((int)unaff_A6 + 0x6e) + 0x80);
  return;
}
