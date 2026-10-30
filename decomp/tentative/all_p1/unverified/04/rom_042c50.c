/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x042c50.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_42c50(void)

{
  int iVar1;
  byte bVar2;
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_1abc8();
  candidate_44f6();
  candidate_427a();
  if ((bool)in_CF) {
    candidate_4489e();
  }
  iVar1 = *(int *)((int)unaff_A6 + 0x6e);
  if ((*(byte *)(iVar1 + 0x15) & 4) == 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfb;
  }
  else {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 4;
  }
  *(undefined2 *)(iVar1 + 0x26) = *(undefined2 *)(iVar1 + 0x26);
  if (*(char *)(*(int *)((int)unaff_A6 + 0x72) + 0x87) != '\0') {
    *(ushort *)((int)unaff_A6 + 0x86) = *(short *)((int)unaff_A6 + 0x86) + 0x180U & 0x3fff;
    func_0x00009cce();
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
