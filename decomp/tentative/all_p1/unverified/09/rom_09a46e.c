/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09a46e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9a46e(void)

{
  uint uVar1;
  undefined1 uVar2;
  uint uVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  *(undefined1 *)((int)unaff_A6 + 0x82) = 0xc;
  candidate_4256();
  unaff_A6[0xd] = 0x4cccc;
  *(undefined4 *)((int)unaff_A6 + 0x7e) = 0xffff0a3e;
  unaff_A6[0xc] = 0xfffe6667;
  *unaff_A6 = 0x9a4a2;
  if (unaff_A6[0xc] != 0) {
    uVar1 = (int)unaff_A6[0xc] >> 4;
    uVar3 = (uint)(short)-(uVar1 == 0);
    unaff_A6[0xc] = unaff_A6[0xc] - ((uVar3 ^ uVar1) - uVar3);
  }
  *(char *)((int)unaff_A6 + 0x82) = *(char *)((int)unaff_A6 + 0x82) + -1;
  if (*(char *)((int)unaff_A6 + 0x82) < '\x01') {
    unaff_A6[0xc] = 0;
    *unaff_A6 = 0x9a4d2;
  }
  uVar1 = unaff_A6[0xd];
  uVar4 = CARRY4(*(uint *)((int)unaff_A6 + 0x7e),uVar1);
  unaff_A6[0xd] = *(uint *)((int)unaff_A6 + 0x7e) + uVar1;
  func_0x000143e4();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = 0x9a000;
    uVar4 = 0;
  }
  uVar2 = candidate_9ab9a();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x83) = uVar2;
    *unaff_A6 = candidate_9a17e;
    uVar4 = 0;
  }
  candidate_c8bec();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_9a5ee;
  }
  return;
}
