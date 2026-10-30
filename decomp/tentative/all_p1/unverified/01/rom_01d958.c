/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01d958.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1d958(void)

{
  short sVar1;
  ushort uVar2;
  short sVar3;
  uint uVar4;
  ushort uVar5;
  undefined4 *unaff_A6;
  bool bVar6;

  candidate_4256();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  candidate_1b1dc();
  uVar4 = candidate_29c6();
  uVar4 = (uVar4 & 0xff00) + 0x4000;
  unaff_A6[0xd] = uVar4;
  *(uint *)((int)unaff_A6 + 0x7e) = -(uVar4 >> 6);
  *(undefined2 *)((int)unaff_A6 + 0x8a) = 0;
  if (*(short *)(unaff_A6 + 0x23) < 0xb) {
    *(undefined2 *)(unaff_A6 + 0x23) = 10;
  }
  *unaff_A6 = 0x1d9c6;
  uVar5 = candidate_29c6();
  uVar2 = uVar5 & 0x3ff;
  if ((short)uVar5 < 1) {
    uVar2 = -uVar2;
  }
  *(short *)((int)unaff_A6 + 0x8a) = uVar2 + *(short *)((int)unaff_A6 + 0x8a);
  sVar1 = *(short *)((short)((ushort)*(byte *)((int)unaff_A6 + 0x8a) * 2) + 0x29fc);
  unaff_A6[0xc] = CONCAT22(sVar1 >> 0xf,sVar1 * 0x200);
  func_0x000143e4();
  uVar4 = unaff_A6[0xd];
  bVar6 = CARRY4(*(uint *)((int)unaff_A6 + 0x7e),uVar4);
  unaff_A6[0xd] = *(uint *)((int)unaff_A6 + 0x7e) + uVar4;
  candidate_427a();
  if (bVar6) {
    *unaff_A6 = candidate_41ac;
  }
  sVar1 = *(short *)(unaff_A6 + 0x23);
  sVar3 = sVar1 + -1;
  *(short *)(unaff_A6 + 0x23) = sVar3;
  if (sVar3 == 0 || 0 >= sVar1) {
    *unaff_A6 = candidate_41ac;
  }
  bVar6 = (sVar3 != 0 && 0 < sVar1) && sVar1 == 0;
  candidate_19504();
  if (bVar6) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
