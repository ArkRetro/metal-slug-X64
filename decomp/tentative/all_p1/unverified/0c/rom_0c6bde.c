/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c6bde.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_c6bde(void)

{
  ushort *puVar1;
  int iVar2;
  short in_D0w;
  ushort uVar3;
  short unaff_D2w;
  ushort unaff_D4w;
  short sVar4;
  int in_A0;
  ushort *puVar5;

  uVar3 = (short)(in_D0w << 4) >> 4;
  puVar5 = *(ushort **)(in_A0 + 0x3e);
  if (puVar5 == (ushort *)0xffffffff) {
    return uVar3;
  }
  iVar2 = *(int *)(in_A0 + 0x2e);
  sVar4 = unaff_D2w + -1;
  do {
    puVar1 = (ushort *)(in_A0 + 0x42 + (int)(short)((unaff_D4w & 0x1f) * 2));
    if (uVar3 < *(ushort *)(iVar2 + 8)) {
      if (puVar5 == (ushort *)0x0) {
        *puVar1 = 0x21;
      }
      else {
        for (; (*puVar5 != 0 && (uVar3 < *puVar5)); puVar5 = puVar5 + -2) {
        }
        for (; puVar5[2] <= uVar3; puVar5 = puVar5 + 2) {
        }
        if (*(byte *)((int)puVar5 + 3) < 0x20) {
          *(byte *)((int)puVar1 + 1) = *(byte *)((int)puVar5 + 3);
          *(char *)puVar1 =
               ((char)*(undefined2 *)(in_A0 + 0x34) + ('\x01' - *(char *)(puVar5 + 1))) * '\b';
        }
        else {
          *puVar1 = 0x21;
        }
      }
    }
    else if ((*(byte *)(in_A0 + 0x82) & 8) != 0) {
      *puVar1 = *puVar1 & 0xff00;
    }
    uVar3 = uVar3 + 1;
    unaff_D4w = unaff_D4w + 1;
    sVar4 = sVar4 + -1;
  } while (sVar4 != -1);
  *(ushort **)(in_A0 + 0x3e) = puVar5;
  return uVar3 - unaff_D2w;
}
