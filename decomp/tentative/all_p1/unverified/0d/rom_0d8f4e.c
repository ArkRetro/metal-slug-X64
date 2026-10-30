/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d8f4e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d8f4e(void)

{
  byte bVar2;
  ushort uVar1;
  undefined1 uVar3;
  int unaff_A6;

  bVar2 = candidate_29c6();
  *(byte *)(unaff_A6 + 0x8d) = bVar2 & 3;
  candidate_d8f14();
  uVar1 = candidate_29c6();
  *(undefined2 *)(unaff_A6 + 0xa8) = *(undefined2 *)((short)((uVar1 & 3) << 1) + 0xe9958);
  uVar3 = candidate_2eb4();
  *(undefined1 *)(unaff_A6 + 0x6e) = uVar3;
  return;
}
