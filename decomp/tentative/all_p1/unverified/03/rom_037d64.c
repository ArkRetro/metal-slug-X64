/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x037d64.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_37d64(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  ushort uVar4;
  short extraout_D1w;
  int unaff_A2;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar5;

  candidate_1ab98();
  candidate_9f88();
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0x37daa;
  bVar5 = false;
  sVar3 = candidate_37c78();
  if (!bVar5) {
    uVar4 = sVar3 - *(short *)(unaff_A6 + 10);
    if ((short)uVar4 < 0) {
      uVar4 = -uVar4;
    }
    if (uVar4 < 0x11) {
      uVar4 = extraout_D1w - *(short *)(unaff_A6 + 0xb);
      if ((short)uVar4 < 0) {
        uVar4 = -uVar4;
      }
      if (uVar4 < 0xb) {
        if (*(char *)(unaff_A2 + 0x5a) == '\0') {
          *(undefined1 *)(unaff_A2 + 0x5a) = 0xff;
        }
        else {
          *(byte *)(unaff_A2 + 0x5f) = *(byte *)(unaff_A2 + 0x5f) | 0x80;
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
    }
  }
  candidate_1ac38();
  candidate_427a();
  candidate_37512();
  return;
}
