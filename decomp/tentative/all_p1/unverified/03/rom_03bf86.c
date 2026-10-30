/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03bf86.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3bf86(void)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  short sVar4;
  uint uVar5;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar6;
  undefined1 uVar7;

  candidate_4256();
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0x78;
  *unaff_A6 = 0x3bfba;
  uVar7 = 0;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    uVar5 = -0xc0000 - unaff_A6[0xd] >> 5;
    uVar3 = unaff_A6[0xd];
    uVar7 = CARRY4(uVar5,uVar3);
    unaff_A6[0xd] = uVar5 + uVar3;
    func_0x000143e4();
  }
  else {
    candidate_1456a();
  }
  candidate_427a();
  if (((bool)uVar7) &&
     (sVar4 = *(short *)((int)unaff_A6 + 0x6e), *(short *)((int)unaff_A6 + 0x6e) = sVar4 + -1,
     sVar4 == 0)) {
    *unaff_A6 = 0x3c000;
  }
  if ((*(ushort *)(unaff_A5 + -0x7ff2) & 3) == 0) {
    if (*(char *)(unaff_A6 + 5) == *(char *)((int)unaff_A6 + 0x19)) {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
    }
    else {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
    }
  }
  bVar6 = false;
  candidate_19504();
  if (bVar6) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
