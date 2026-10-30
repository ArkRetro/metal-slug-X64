/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04876c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4876c(void)

{
  int iVar1;
  short sVar2;
  undefined4 *unaff_A6;
  bool bVar3;

  *(undefined2 *)(unaff_A6 + 0x1d) = *(undefined2 *)(unaff_A6[3] + 0x74);
  candidate_4982a();
  *unaff_A6 = 0x48780;
  candidate_499a4();
  candidate_49a92();
  candidate_1ac38();
  bVar3 = false;
  candidate_49e46();
  if (bVar3) {
    *unaff_A6 = candidate_487d0;
  }
  iVar1 = unaff_A6[3];
  sVar2 = (ushort)*(byte *)((int)unaff_A6 + 0x61) * 4 + 0x76;
  *(undefined2 *)(iVar1 + sVar2) = *(undefined2 *)(unaff_A6 + 0xf);
  *(undefined2 *)(iVar1 + 2 + (int)sVar2) = *(undefined2 *)(unaff_A6 + 0x10);
  *(undefined1 *)(iVar1 + (short)(*(byte *)((int)unaff_A6 + 0x61) + 0x90)) =
       *(undefined1 *)((int)unaff_A6 + 0xaa);
  candidate_490ae();
  return;
}
