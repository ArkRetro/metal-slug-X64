/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0465fe.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_465fe(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  ushort uVar4;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar5;
  undefined8 uVar6;

  candidate_9ea8();
  candidate_9ea8();
  candidate_9ea8();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_4256();
  uVar6 = candidate_3b332();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar6;
  *unaff_A6 = 0x46678;
  candidate_143dc();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
    *unaff_A6 = candidate_466dc;
  }
  sVar3 = candidate_49e8();
  if (sVar3 != 0) {
    *unaff_A6 = candidate_466f6;
  }
  uVar4 = _DAT_0010000e & 3;
  if (2 < uVar4) {
    uVar4 = 1;
  }
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + (short)uVar4 + 0x19);
  bVar5 = false;
  candidate_19504();
  if (bVar5) {
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
