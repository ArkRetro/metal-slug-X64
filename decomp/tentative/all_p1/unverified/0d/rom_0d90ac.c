/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d90ac.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d90ac(void)

{
  undefined2 uVar1;
  ushort uVar2;
  byte bVar3;
  ushort uVar4;
  int unaff_A6;
  undefined1 uVar5;

  uVar1 = *(undefined2 *)(unaff_A6 + 0x2c);
  bVar3 = *(byte *)(unaff_A6 + 0x7e);
  *(char *)(unaff_A6 + 0x7e) = *(char *)(unaff_A6 + 0x7e) + -0x80;
  uVar4 = *(short *)((short)((ushort)bVar3 * 2) + 0x29fc) >> 8;
  *(ushort *)(unaff_A6 + 0x92) = uVar4;
  uVar2 = *(ushort *)(unaff_A6 + 0x2c);
  uVar5 = CARRY2(uVar4,uVar2);
  *(ushort *)(unaff_A6 + 0x2c) = uVar4 + uVar2;
  candidate_1abc8();
  candidate_427a();
  if (!(bool)uVar5) {
    *(undefined2 *)(unaff_A6 + 0x2c) = uVar1;
    return;
  }
  *(undefined2 *)(unaff_A6 + 0x2c) = uVar1;
  return;
}
