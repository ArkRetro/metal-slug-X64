/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x053d16.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_53d16(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  func_0x000143e4();
  if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) &&
     ((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0)) {
    candidate_427a();
    sVar3 = candidate_49e8();
    if (sVar3 != 0) goto LAB_00053d6e;
  }
  else {
LAB_00053d6e:
    if (unaff_A6[0xd] == 0) {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    unaff_A6[0xc] = 0;
    unaff_A6[0xd] = 0;
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
    *unaff_A6 = 0x53dae;
    bVar4 = false;
    candidate_427a();
    if (bVar4) goto candidate_4188;
  }
  if (((ushort)(*(short *)(unaff_A6 + 0xf) + 0x10U) < 0x160) &&
     ((ushort)(*(short *)(unaff_A6 + 0x10) - 0xf0U) < 0x120)) {
    candidate_505a();
    return;
  }
candidate_4188:
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
