/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08471a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8471a(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_9f88();
  candidate_9ea8();
  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_8508e();
  *(undefined2 *)((int)unaff_A6 + 0x8a) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x82) = 10;
  *unaff_A6 = 0x8475e;
  iVar2 = *(int *)((int)unaff_A6 + 0x7e) + unaff_A6[0xd];
  unaff_A6[0xd] = iVar2;
  if (iVar2 < 0) {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
    func_0x000143e4();
  }
  else {
    candidate_1ac38();
  }
  candidate_427a();
  sVar3 = candidate_2f1a();
  *(short *)((int)unaff_A6 + 0x8a) = sVar3 + *(short *)((int)unaff_A6 + 0x8a);
  if (0x11ff < *(short *)((int)unaff_A6 + 0x8a)) {
    *(short *)((int)unaff_A6 + 0x8a) = *(short *)((int)unaff_A6 + 0x8a) + -0x1200;
    candidate_40bc();
    candidate_1acf2();
  }
  uVar4 = 0;
  if (*(short *)((int)unaff_A6 + 0x82) == 0) {
    candidate_49e8();
  }
  else {
    sVar3 = *(short *)((int)unaff_A6 + 0x82);
    uVar4 = sVar3 == 0;
    *(short *)((int)unaff_A6 + 0x82) = sVar3 + -1;
  }
  candidate_1945c();
  if (!(bool)uVar4) {
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *unaff_A6 = candidate_8480e;
    }
    return;
  }
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
