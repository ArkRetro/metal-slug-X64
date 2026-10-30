/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x080ce0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_80ce0(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;
  bool bVar5;
  undefined8 uVar6;

  candidate_9ea8();
  candidate_9ea8();
  candidate_9ea8();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  unaff_A6[0x14] = 0xe4998;
  uVar6 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar6;
  *(undefined4 *)((int)unaff_A6 + 0x72) = 0xe49a4;
  *unaff_A6 = 0x80d62;
  uVar4 = 0;
  func_0x000143e4();
  candidate_1945c();
  if (!(bool)uVar4) {
    if (((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0) &&
       ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0)) {
      sVar3 = candidate_49e8();
      if (sVar3 == 0) {
        *(undefined4 *)((int)unaff_A6 + 0x72) = 0xe49b4;
        candidate_427a();
        return;
      }
    }
    bVar5 = false;
    candidate_40bc();
    if (bVar5) {
      candidate_1b220();
    }
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
